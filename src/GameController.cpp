#include "../include/GameController.h"

GameController::GameController(size_t smallBlind, size_t bigBlind) :
    smallBlind(smallBlind),
    bigBlind(bigBlind),
    roundNum(0),
    deck(),
    board(),
    dealer(Dealer::createDealer(deck, board)),
    gamePlayers(),
    actionManager(),
    turnManager(),
    clientManager(bigBlind),
    potManager() {}

// GAME STATE INFORMATION METHODS

void GameController::displayPlayersInGame() const {
    gamePlayers.displayPlayersInGame();
}

// PLAYER SPECIFIC METHODS

void GameController::addNewPlayer(const string& name, size_t chips) {
    shared_ptr<Player> newPlayer = gamePlayers.addPlayerToGame(name, chips);
    if (newPlayer != nullptr) turnManager.addPlayerInHand(newPlayer);
}

void GameController::removePlayer(const string& name) {
    shared_ptr<Player> oldPlayer = gamePlayers.removePlayerFromGame(name);
    if (oldPlayer != nullptr) turnManager.removePlayerFromHand(oldPlayer);
}
// STREET SPECIFIC METHODS

void GameController::startStreet(Street newStreet) {
    if (isFoldedThrough()) {
        cout << "Players folded through. Skipping " << streetToStr(newStreet) << endl;
        return;
    }

    if (isPlayersInHandAllIn()) {
        cout << "Players in the hand are all in. Skipping " << streetToStr(newStreet) << endl;
        return;
    }

    cout << "\nStarting " << streetToStr(newStreet) << " Street\n" << endl;

    setupStreet(newStreet);
    int initialPlayersInHand = turnManager.getNumPlayersInHand();

    while (!isStreetOver(initialPlayersInHand)) {
        // Get player to act
        shared_ptr<Player> curPlayer = turnManager.getPlayerToAct();

        // Get possible actions for player
        vector<PossibleAction> possibleActions = actionManager.getAllowedActionTypes();

        // Request client action given possible actions
        size_t initialChips = curPlayer->getChips() + potManager.getRecentBet(curPlayer); // Represents the number of chips the player started the street with!
        ClientAction clientAction = clientManager.getClientAction(curPlayer, possibleActions, initialChips);

        // Add action to the action timeline
        shared_ptr<Action> playerAction = createAction(clientAction);
        actionManager.addActionToTimeline(playerAction);

        // Update the pot manager after action
        ActionType playerActionType = playerAction->getActionType();
        if (playerActionType == BET || playerActionType == RAISE || playerActionType == CALL || playerActionType == BLIND) {
            potManager.addPlayerBet(curPlayer, playerAction->getAmount());
        } else if (playerActionType == FOLD) {
            potManager.foldPlayerBet(curPlayer);
            turnManager.foldPlayerInHand(curPlayer);
        }
    }

    // Clear action timeline
    actionManager.clearActionTimeline();

    // Calculate pots
    potManager.calculatePots();

    // Reset recent bets and dead money
    potManager.resetPlayerBets();
}

// ROUND SPECIFIC METHODS

void GameController::startRound() {
    cout << "Beginning new round of Texas Hold'Em!\n" << endl;
    startStreet(PRE_FLOP);
    startStreet(FLOP);
    startStreet(TURN);
    startStreet(RIVER);

    potManager.displayPots();
    cout << "Round Finished!\n" << endl;
}

void GameController::main() {

    // Before each round: Add / Remove players (Y/N)
    // For players with less chips than BB, ask players to add chips or remove from game
    // Check if there are enough players to start the game
    // startRound
}

// HELPER FUNCTIONS

string GameController::streetToStr(Street street) {
    switch(street) {
        case PRE_FLOP:
            return "Pre-Flop";
        case FLOP:
            return "Flop";
        case TURN:
            return "Turn";
        case RIVER:
            return "River";
        case SHOWDOWN:
            return "Showdown";
        default:
            return "Unknown Street";
    }
}

void GameController::dealPlayers() {
    cout << "--------Dealing cards to players!-------" << endl;
    for (int i = 0; i < 2; i++) {
        for (const auto& player : turnManager.getPlayersInHand()) {
            dealer.dealPlayer(player);
        }
    }
    cout << "----------------------------------------\n" << endl;
}

void GameController::dealBoard(int numCards) {
    cout << "----------------------------------------" << endl;
    cout << "Dealing " << numCards << " community card(s) to the board!" << endl;
    dealer.dealBoard(numCards, 1);
    cout << "----------------------------------------\n" << endl;
}

void GameController::setupStreet(Street newStreet) {
    if (newStreet == PRE_FLOP) {
        // Deal players hole cards
        dealPlayers();

        // Small blind
        turnManager.setSmallBlindToAct();
        auto smallBlindPlayer= turnManager.getPlayerToAct();
        auto postSmallBlind = make_shared<BlindAction>(smallBlindPlayer, smallBlind);
        
        actionManager.addActionToTimeline(postSmallBlind);
        potManager.addPlayerBet(smallBlindPlayer, smallBlind);

        // Big blind
        auto bigBlindPlayer = turnManager.getPlayerToAct();
        auto postBigBlind = make_shared<BlindAction>(bigBlindPlayer, bigBlind);

        actionManager.addActionToTimeline(postBigBlind);
        potManager.addPlayerBet(bigBlindPlayer, bigBlind);

    } else if (newStreet == FLOP) {
        dealBoard(3);
        turnManager.setEarlyPositionToAct();
    } else if (newStreet == TURN || newStreet == RIVER) {
        dealBoard(1);
        turnManager.setEarlyPositionToAct();
    }
}

shared_ptr<Action> GameController::createAction(const ClientAction& action) {
    switch (action.type) {
        case BET:
            return make_shared<BetAction>(action.player, action.amount);
        case BLIND:
            return make_shared<BlindAction>(action.player, action.amount);
        case CALL:
            return make_shared<CallAction>(action.player, action.amount);
        case CHECK:
            return make_shared<CheckAction>(action.player);
        case FOLD:
            return make_shared<FoldAction>(action.player);
        case RAISE:
            return make_shared<RaiseAction>(action.player, action.amount);
        default:
            runtime_error("Error trying to create an action object from client input!");
            return nullptr;
    }
}

bool GameController::isStreetOver(int initialPlayersInHand) {
    return actionManager.isActionsFinished(initialPlayersInHand);
}

bool GameController::isPlayersInHandAllIn() {
    for (auto const& player : turnManager.getPlayersInHand()) {
        if (player->getChips() != 0) return false;
    }
    return true;
}

bool GameController::isFoldedThrough() {
    return (turnManager.getNumPlayersInHand() == 1);
}

void GameController::setupNewRound() {
    // Reset folded players
    turnManager.moveFoldedPlayersToInHand();

    // Rotate positions
    turnManager.rotatePositions();

    // Clear action timeline
    actionManager.clearActionTimeline();

    // Reset player bets and dead money
    potManager.resetPlayerBets();
}

bool GameController::isEnoughPlayersInGame() {
    return (gamePlayers.getNumPlayersInGame() > 1);
}