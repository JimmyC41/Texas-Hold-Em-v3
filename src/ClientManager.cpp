#include "../include/ClientManager.h"
#include "../include/ActionManager.h"
#include "../include/Action.h"

ClientManager::ClientManager(size_t bigBlind) : bigBlind(bigBlind) {}

ClientAction ClientManager::getClientAction(shared_ptr<Player>& playerToAct, vector<PossibleAction>& possibleActions) {
    displayPossibleActions(playerToAct, possibleActions);

    // Fetch action type from client
    ActionType clientActionType = getClientActionType(possibleActions);

    // Fetch bet amount from client
    size_t amount = getClientBetAmount(playerToAct, clientActionType, possibleActions, bigBlind);

    ClientAction clientAction = ClientAction{playerToAct, clientActionType, amount};
    displayClientAction(clientAction);

    return clientAction;
}

// Client Helper Function

ActionType ClientManager::getClientActionType(vector<PossibleAction>& possibleActions) {
    string actionStr;
    ActionType actionType;

    while (true) {
        cout << "Please enter a valid action: ";
        getline(cin, actionStr);

        actionType = strToActionType(actionStr);
        if (isValidAction(possibleActions, actionType)) {
            cout << "   Client option: " << actionStr << endl;;
            break;
        }
    }

    return actionType;
}

size_t ClientManager::getClientBetAmount(shared_ptr<Player>& playerToAct, ActionType clientAction, vector<PossibleAction>& possibleActions, size_t bigBlind) {
    // Case 1: Check or Fold (Bet Amount is 0)
    if (clientAction == ActionType::CHECK || clientAction == ActionType::FOLD) {
        return 0;
    }

    size_t maxBet = playerToAct->getChips();

    // Case 2: Call (Call amount is previous bet amount)
    if (clientAction == ActionType::CALL) {
        size_t callAmount = getRelevantBet(possibleActions, clientAction);
        if (callAmount >= maxBet) callAmount = maxBet; // Edge case where player is all in to call
        return callAmount;
    }

    // Case 3: Bet or Raise (Bet amount determined by the client)
    
    size_t minBet = bigBlind; // Minimum bet is set to big blind by default

    // Set minimum bet if the client wishes to raise
    if (clientAction == ActionType::RAISE) {
        size_t prevBetAmount = getRelevantBet(possibleActions, clientAction);

        cout << "Previous bet amount is: " << prevBetAmount << endl;

        // Assume that minimum raise is the previous bet
        minBet = 2 * prevBetAmount;

        // Edge case where player is all in to raise (player does not have a choice)
        if (minBet >= maxBet) return maxBet;
    }

    // Fetch client bet amount from stdin
    size_t amount;
    while (true) {
        cout << "Please enter a bet size of [" << minBet << ", " << maxBet << "]: ";
        cin >> amount;

        if (cin.fail()) {
            cout << "Please enter a valid number!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            if (isValidAmount(amount, minBet, maxBet)) return amount;
        }
    }
}

// Helper Functions

void ClientManager::displayPossibleActions(shared_ptr<Player>& playerToAct, vector<PossibleAction>& possibleActions) {
    cout << "Displaying possible actions for" << playerToAct->getName() << ":" << endl;
    ActionManager::displayPossibleActions(possibleActions);
}

void ClientManager::displayClientAction(const ClientAction& clientAction) {
    cout << "Displaying ClientAction object attributes:" << endl;
    cout << "   Player: " << clientAction.player->getName() << endl;
    cout << "   Action Type: " << Action::actionTypeToStr(clientAction.type) << endl;
    cout << "   Amount: " << clientAction.amount << endl;
}

ActionType ClientManager::strToActionType(string& str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);

    if (str == "bet") return ActionType::BET;
    if (str == "check") return ActionType::CHECK;
    if (str == "fold") return ActionType::FOLD;
    if (str == "call") return ActionType::CALL;
    if (str == "raise") return ActionType::RAISE;
    if (str == "blind") return ActionType::BLIND;
    else return ActionType::INVALID_ACTION;
}

bool ClientManager::isValidAction(vector<PossibleAction>& possibleActions, ActionType chosenAction) {
    if (chosenAction == INVALID_ACTION) return false;

    auto it = find_if(possibleActions.begin(), possibleActions.end(), [chosenAction](const PossibleAction& action) {
                return action.type == chosenAction;
            });
    
    if (it != possibleActions.end()) return true;
    else return false;
}

size_t ClientManager::getRelevantBet(vector<PossibleAction>& possibleActions, ActionType chosenAction) {
    for (const auto& action : possibleActions) {
        if (action.type == chosenAction) return action.amount;
    }

    cout << "Could not find a relevant bet for the chosen action!" << endl;
    return 0;
}

bool ClientManager::isValidAmount(size_t amount, size_t min, size_t max) {
    if (amount >= min && amount <= max) return true;

    cout << "Entered a bet amount outside of the provided range!" << endl;
    return false;
}