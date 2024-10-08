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
int GameController::getNumPlayersInGame() const {
    return gamePlayers.getNumPlayersInGame();
}

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

// DEALER SPECIFIC METHODS

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

// STREET SPECIFIC METHODS

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

bool GameController::isFoldedThrough() {
    return (turnManager.getNumPlayersInHand() == 1);
}

void GameController::startStreet(Street newStreet) {
    cout << "\nStarting " << streetToStr(newStreet) << " Street\n" << endl;

    if (isFoldedThrough()) {
        cout << "Players folded through. Skipping " << streetToStr(newStreet) << endl;
        return;
    }

    setupStreet(newStreet);
    int initialPlayersInHand = turnManager.getNumPlayersInHand();

    while (!isStreetOver(initialPlayersInHand)) {
        // Get player to act
        shared_ptr<Player> curPlayer = turnManager.getPlayerToAct();

        // Get possible actions for player
        vector<PossibleAction> possibleActions = actionManager.getAllowedActionTypes();

        // Request client action given possible actions
        ClientAction clientAction = clientManager.getClientAction(curPlayer, possibleActions);

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
    potManager.displayPots();

    // Reset recent bets and dead money
    potManager.resetPlayerBets();
}

// ROUND SPECIFIC METHODS

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

void GameController::startRound() {
    cout << "Beginning new round of Texas Hold'Em!\n" << endl;
    startStreet(PRE_FLOP);
    startStreet(FLOP);
    startStreet(TURN);
    startStreet(RIVER);
    cout << "Round Finished!\n" << endl;
}

// HELPER

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