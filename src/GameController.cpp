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
    potManager(),
    handEvaluator() {}

// GAME STATE INFORMATION METHODS

void GameController::displayPlayersInGame() const {
    gamePlayers.displayPlayersInGame();
}

// PLAYER SPECIFIC METHODS

void GameController::queryNewPlayer() {
    while (true) {
        cout << "Would you like to add a new player? (y/n): ";
        string input;
        cin >> input;

        transform(input.begin(), input.end(), input.begin(), ::tolower);

        if (input != "y") {
            cout << "No more players will be added!" << endl;
            break;
        }

        string name = queryPlayerName();
        size_t chips = queryPlayerChips();

        shared_ptr<Player> newPlayer = gamePlayers.addPlayerToGame(name, chips);
        if (newPlayer != nullptr) turnManager.addPlayerInHand(newPlayer);
    }
}

void GameController::queryRemovePlayer() {
    while (true) {
        cout << "Would you like to remove an existing player? (y/n: ";
        string input;
        cin >> input;

        transform(input.begin(), input.end(), input.begin(), ::tolower);

        if (input != "y") {
            cout << "No more players will be removed.\n" << endl;
            break;
        }

        string playerName = queryPlayerName();

        shared_ptr<Player> oldPlayer = gamePlayers.removePlayerFromGame(playerName);
        if (oldPlayer != nullptr) turnManager.removePlayerFromHand(oldPlayer);
    }
}

void GameController::validateChipCounts() {
    for (auto& player : gamePlayers.getGamePlayers()) {
        if (player->getChips() < bigBlind) {
            cout << "Player " << player->getName() << " has less than the minimum required chips (" << bigBlind << "). Current chips: " << player->getChips() << endl;
            size_t chipsToAdd = queryChipsToAdd(bigBlind, player->getChips());
            player->addChips(chipsToAdd);
            cout << "Player " << player->getName() << " now has " << player->getChips() << " chips." << endl;
        }
        else {
            cout << "Player " << player->getName() << "has sufficient chips to continue." << endl;
        }
    }
}

// STREET SPECIFIC METHODS

void GameController::startStreet(Street newStreet) {
    if (isNoMoreAction()) {
        cout << "No more players to act. Skipping " << streetToStr(newStreet) << endl;
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
        
        size_t activeBet = actionManager.getActiveBet();
        size_t initialChips = potManager.getInitialChips(curPlayer); // Chips the player to act started with
        size_t bigStackAmongOthers = potManager.getBigStackAmongOthers(curPlayer, turnManager.getPlayersInHand()); // Chips of the biggest stack among other active players

        bool playerCanRaise = true;
        // Player cannot raise under the following circumstances (respectively)
        // The player is all in to call (i.e. initial chips < active bet)
        // The player is the biggest stack, and the next biggest stack has gone all in
        if (initialChips < activeBet || activeBet >= bigStackAmongOthers && initialChips > bigStackAmongOthers) {
            playerCanRaise = false;
            cout << "Player can NOT raise!" << endl;
            cout << "Player's initial chips and active bet:" << initialChips << activeBet << endl;
        }

        // Get possible actions for player
        vector<PossibleAction> possibleActions = actionManager.getAllowedActionTypes(playerCanRaise);

        // Request client action given possible actions
        bool isPreFlop = false;
        if (newStreet == Street::PRE_FLOP) isPreFlop = true;
        ClientAction clientAction = clientManager.getClientAction(isPreFlop, curPlayer, possibleActions, initialChips, bigStackAmongOthers);

        // Add action to the action timeline
        shared_ptr<Action> playerAction = createAction(clientAction, initialChips);
        actionManager.addActionToTimeline(playerAction);

        // Update the pot manager after action
        ActionType playerActionType = playerAction->getActionType();
        if (playerActionType == BET || playerActionType == RAISE || playerActionType == CALL || playerActionType == BLIND) {
            potManager.addPlayerBet(curPlayer, playerAction->getAmount(), false);
        } else if (playerActionType == FOLD) {
            potManager.foldPlayerBet(curPlayer);
            turnManager.addPlayerNotInHand(curPlayer);
        } else if (playerActionType == ALL_IN_BET || playerActionType == ALL_IN_CALL) {
            potManager.addPlayerBet(curPlayer, playerAction->getAmount(), true);
            turnManager.addPlayerNotInHand(curPlayer);
        }

        // cout << curPlayer->getName() << " chip count: " << curPlayer->getChips() << endl;
        // potManager.displayPlayerBets();
        turnManager.displayPlayerChipCount();
    }

    // Clear action timeline
    actionManager.clearActionTimeline();

    // Calculate pots
    potManager.calculatePots();

    // Reset player bets in pots
    potManager.resetPlayerBets();
}

// ROUND SPECIFIC METHODS

void GameController::startRound() {
    startStreet(PRE_FLOP);
    startStreet(FLOP);
    startStreet(TURN);
    startStreet(RIVER);

    // No more action! Evaluate hands and award pots
    potManager.displayPots();
    populatePlayerHandsMap();
    vector<shared_ptr<Player>> sortedPlayers = handEvaluator.getSortedPlayers();
    potManager.awardPots(sortedPlayers);

    cout << "Round completed!\n" << endl;
}

void GameController::populatePlayerHandsMap() {
    for (const auto& player : gamePlayers.getGamePlayers()) {
        // Add hole cards
        for (const auto& holeCard : player->getHand()) {
            handEvaluator.addDealtCard(player, holeCard);
        }
        // Add community cards
        for (const auto& communityCard : board.getCommunityCards()) {
            handEvaluator.addDealtCard(player, communityCard);
        }
    }
}

// GAME SPECIFIC METHODS

void GameController::main() {
    int roundNum = 0;
    while (verifyGamePlayers()) {
        cout << "Beginning round #" << roundNum++ << " of Texas Hold'Em!\n" << endl;
        startRound();
        setupNewRound();
    }
    cout << "Ending the application! Game Over!\n" << endl;
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
        potManager.addPlayerBet(smallBlindPlayer, smallBlind, false);

        // Big blind
        auto bigBlindPlayer = turnManager.getPlayerToAct();
        auto postBigBlind = make_shared<BlindAction>(bigBlindPlayer, bigBlind);

        actionManager.addActionToTimeline(postBigBlind);
        potManager.addPlayerBet(bigBlindPlayer, bigBlind, false);

    } else if (newStreet == FLOP) {
        dealBoard(3);
        turnManager.setEarlyPositionToAct();
    } else if (newStreet == TURN || newStreet == RIVER) {
        dealBoard(1);
        turnManager.setEarlyPositionToAct();
    }
}

shared_ptr<Action> GameController::createAction(const ClientAction& action, size_t initialChips) {
    switch (action.type) {
        case BET:
            if (action.amount == initialChips) {
                return make_shared<AllInBetAction>(action.player, action.amount);
            }
            return make_shared<BetAction>(action.player, action.amount);
        case BLIND:
            return make_shared<BlindAction>(action.player, action.amount);
        case CALL:
            if (action.amount == initialChips) {
                cout << "All in Call triggered" << endl;
                return make_shared<AllInCallAction>(action.player, action.amount);
            }
            cout << "Just a normal call?" << endl;
            return make_shared<CallAction>(action.player, action.amount);
        case CHECK:
            return make_shared<CheckAction>(action.player);
        case FOLD:
            return make_shared<FoldAction>(action.player);
        case RAISE:
            if (action.amount == initialChips) {
                return make_shared<AllInBetAction>(action.player, action.amount);
            }
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

bool GameController::isNoMoreAction() {
    return (turnManager.getNumPlayersInHand() == 1);
}

void GameController::setupNewRound() {
    // Reset folded players
    turnManager.moveAllPlayersToInHand();

    // Rotate positions
    turnManager.rotatePositions();

    // Clear action timeline
    actionManager.clearActionTimeline();

    // Reset player bets and dead money
    // Reset pots
    potManager.resetPlayerBets();
    potManager.resetPots();

    // New deck and clear board
    dealer.resetDeck();
    dealer.resetBoard();

    // Clear the playerHands map
    handEvaluator.clearHandEvaluator();
}

bool GameController::verifyNumPlayers() {
    if (gamePlayers.getNumPlayersInGame() < 2) {
        cout << "Not enough players to start the game." << endl;
        return false;
    }
    return true;
}

bool GameController::verifyGamePlayers() {
    queryNewPlayer();
    queryRemovePlayer();
    validateChipCounts();
    return verifyNumPlayers();
}

string GameController::queryPlayerName() {
    string name;
    do {
        cout << "Enter player's name: ";
        cin >> name;
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (name.empty()) {
            cout << "Invalid input. Please enter a valid name." << endl;
        }
    } while (name.empty()); // Repeat until a non-empty name is provided
    return name;
}

size_t GameController::queryPlayerChips() {
    size_t chips;
    while (true) {
        cout << "Enter player's chips: ";
        cin >> chips;
        
        // Check if the input is valid and a positive number
        if (cin.fail() || chips <= 0) {
            cout << "Invalid input. Please enter a valid number of chips." << endl;
            cin.clear(); // Clear the error flag on cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        } else {
            break; // Valid input received
        }
    }
    return chips;
}

// Helper function to query the number of chips to add
size_t GameController::queryChipsToAdd(size_t minChips, size_t currentChips) const {
    size_t chipsToAdd;
    while (true) {
        cout << "Enter the number of chips to add (must be at least "
             << (minChips - currentChips) << "): ";
        cin >> chipsToAdd;

        if (cin.fail() || chipsToAdd <= 0 || (currentChips + chipsToAdd) < minChips) {
            cout << "Invalid input. Please enter a valid number of chips." << endl;
            cin.clear(); // Clear the error flag on cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        } else {
            break; // Valid input received
        }
    }
    return chipsToAdd;
}
