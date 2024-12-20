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
    handEvaluator(),
    streetState() {}

// STREET SPECIFIC METHODS

// Move to turnManager?
// Change name to: canStartNewStreet
bool GameController::isNoMoreAction(Street newStreet) {
    // If there is only one player left in the hand, betting action is complete!
    if (turnManager.getNumPlayersInHand() == 1) {
        cout << "No more players to act. Skipping " << streetToStr(newStreet) << endl;
        return true;
    }

    // If all players are all in, betting action is complete!
    bool allPlayersAllIn = true;
    for (auto const& player : turnManager.getPlayersInHand()) {
        if (player->getChips() != 0) allPlayersAllIn = false;
    }

    if (allPlayersAllIn) {
        cout << "Players in the hand are all in. Skipping " << streetToStr(newStreet) << endl;
        return true;
    } else {
        return false;
    }
}

// While loop can be simplified to:
// Display info
// Ask info
// Process info
void GameController::startStreet(Street newStreet) {
    if (isNoMoreAction(newStreet)) return;
    setupStreet(newStreet); // UPDATE GAME STATE

    while (!isStreetOver(streetState.getInitialNumPlayers())) {

        // Get player to act and fetch possible actions
        shared_ptr<Player> curPlayer = turnManager.getPlayerToAct(); // UPDATE GAME STATE
        udpateStreetStateForCurPlayer(curPlayer);
        vector<PossibleAction> possibleActions = actionManager.getAllowedActionTypes(streetState.getPlayerCanRaise()); // UPDATE GAME STATE

        // DISPLAY GAME STATE
        // REQUEST CLIENT INPUT

        // Request client action given possible actions
        ClientAction clientAction = clientManager.getClientAction(streetState, possibleActions);

        // Process the new action object in ActionManager, potManager and turnManager
        // Refactor: We can chuck the client Action inside processNewAction
        shared_ptr<Action> playerAction = createAction(clientAction, streetState.getPlayerInitialChips());
        processNewAction(curPlayer, playerAction); // UPDATE GAME STATE
    }

    cleanupStreet(); // UPDATE GAME STATE
}

void GameController::setupStreet(Street newStreet) {
    cout << "\nStarting " << streetToStr(newStreet) << " Street\n" << endl;

    streetState.setStreet(newStreet);
    streetState.setInitialNumPlayers(turnManager.getNumPlayersInHand());

    if (newStreet == PRE_FLOP) {
        dealPlayers();
        handleBlind(turnManager, actionManager, potManager, smallBlind, true);
        handleBlind(turnManager, actionManager, potManager, bigBlind, false);
    } else if (newStreet == FLOP) {
        dealBoard(3);
        turnManager.setEarlyPositionToAct();
    } else if (newStreet == TURN || newStreet == RIVER) {
        dealBoard(1);
        turnManager.setEarlyPositionToAct();
    }
}

inline void handleBlind(TurnManager& turnManager, ActionManager& actionManager, PotManager& potManager, int blindAmount, bool isSmallBlind) {
    if (isSmallBlind) turnManager.setSmallBlindToAct();
    auto player = turnManager.getPlayerToAct();
    auto blindAction = std::make_shared<BlindAction>(player, blindAmount);
    actionManager.addActionToTimelineAndUpdateActionState(blindAction);
    potManager.addPlayerBet(player, blindAmount, false);
}

void GameController::processNewAction(shared_ptr<Player>& player, const shared_ptr<Action>& playerAction) {
    actionManager.addActionToTimelineAndUpdateActionState(playerAction);

    ActionType playerActionType = playerAction->getActionType();
    switch (playerActionType) {
        case BET:
        case RAISE:
        case CALL:
        case BLIND:
            potManager.addPlayerBet(player, playerAction->getAmount(), false);
            break;
        case FOLD:
            potManager.foldPlayerBet(player);
            turnManager.addPlayerNotInHand(player);
            break;
        case ALL_IN_BET:
        case ALL_IN_CALL:
            potManager.addPlayerBet(player, playerAction->getAmount(), true);
            turnManager.addPlayerNotInHand(player);
            break;
        default:
            break;
    }
}

void GameController::cleanupStreet() {
    // Reset street state
    streetState.resetStreetState();

    // Clear action timeline and reset action state
    actionManager.clearActionTimelineAndResetActionState();

    // Calculate pots and reset player bets
    potManager.calculatePots();
    potManager.resetPlayerBets();
}

// ROUND SPECIFIC METHODS

void GameController::startRound() {
    // DISPLAY THE GAME STATE TO THE CLIENT
    startStreet(PRE_FLOP);
    startStreet(FLOP);
    startStreet(TURN);
    startStreet(RIVER);
    evaluatePots(); // UPDATE STATE
    // DISPLAY STATE
    cout << "Round completed!\n" << endl;
}

void GameController::evaluatePots() {
    potManager.displayPots();
    populatePlayerHandsMap();
    vector<shared_ptr<Player>> sortedPlayers = handEvaluator.getSortedPlayers();
    potManager.awardPots(sortedPlayers);
}

// Move to handEvaluator
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

string GameController::streetToStr(Street street) {
    switch(street) {
        case PRE_FLOP: return "Pre-Flop";
        case FLOP: return "Flop";
        case TURN: return "Turn";
        case RIVER: return "River";
        case SHOWDOWN: return "Showdown";
        default: return "Unknown Street";
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

// Move to Client Manager
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
                return make_shared<AllInCallAction>(action.player, action.amount);
            }
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

// Move to actionManager
bool GameController::isStreetOver(int initialPlayersInHand) {
    return actionManager.isActionsFinished(initialPlayersInHand);
}

void GameController::udpateStreetStateForCurPlayer(const shared_ptr<Player>& player) {
    streetState.setCurPlayer(player);
    streetState.setActiveBet(actionManager.getActiveBet());
    streetState.setPlayerInitialChips(potManager.getInitialChips(player));
    streetState.setBigStackAmongOthers(potManager.getBigStackAmongOthers(player, turnManager.getPlayersInHand()));

    if ((streetState.getCurPlayer()->getPosition() == Position::BIG_BLIND) &&
        (streetState.getStreet() == Street::PRE_FLOP)) {
            streetState.setIsBigBlindPreFlop(true);
    } else {
            streetState.setIsBigBlindPreFlop(false);
    }
    

    if (streetState.getActiveBet() > streetState.getPlayerInitialChips()) {
        cout << "Player does not have sufficient chips to raise!" << endl;
        streetState.setPlayerCanRaise(false);
        return;
    }
    if ((streetState.getPlayerInitialChips() > streetState.getBigStackAmongOthers()) &&
        (streetState.getActiveBet() >= streetState.getBigStackAmongOthers())) {
        cout << "Player can not raise because the next biggest stack is already all-in to match the active bet!" << endl;
        streetState.setPlayerCanRaise(false);
        return;
    }

    streetState.setPlayerCanRaise(true);
}

void GameController::setupNewRound() {
    // Reset street state
    streetState.resetStreetState();

    // Clear action timeline
    actionManager.clearActionTimelineAndResetActionState();

    // Reset folded players and rotate positions
    turnManager.moveAllPlayersToInHand();
    turnManager.rotatePositions();

    // Reset player bets, dead money and pots
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

// STDOUT STUFF

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

// TO DELETE LATER:

// GAME STATE INFORMATION METHODS

void GameController::displayPlayersInGame() const {
    gamePlayers.displayPlayersInGame();
}