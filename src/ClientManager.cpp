#include "../include/ClientManager.h"
#include "../include/ActionManager.h"
#include "../include/TurnManager.h"
#include "../include/Action.h"
#include <algorithm>

ClientManager::ClientManager(size_t bigBlind) : bigBlind(bigBlind) {}

ClientAction ClientManager::getClientAction(shared_ptr<Player>& playerToAct, vector<PossibleAction>& possibleActions, size_t initialChips, size_t bigStackChips) {
    displayPossibleActions(playerToAct, possibleActions);

    // Fetch action type from client
    bool isBigBlind = false;
    if (playerToAct->getPosition() == Position::BIG_BLIND) isBigBlind = true;
    ActionType clientActionType = getClientActionType(possibleActions, isBigBlind);

    // Fetch bet amount from client
    size_t amount = getClientBetAmount(playerToAct, clientActionType, possibleActions, bigBlind, initialChips, bigStackChips);

    ClientAction clientAction = ClientAction{playerToAct, clientActionType, amount};
    // displayClientAction(clientAction);

    cout << "\n" << endl;
    return clientAction;
}

// Client Helper Function

ActionType ClientManager::getClientActionType(vector<PossibleAction>& possibleActions, bool isBigBlind) {
    string actionStr;
    ActionType actionType;

    while (true) {
        cout << "Please enter a valid action: ";
        getline(cin, actionStr);

        actionType = strToActionType(actionStr, isBigBlind);
        if (isValidAction(possibleActions, actionType)) {
            break;
        }
    }
    return actionType;
}

size_t ClientManager::getClientBetAmount(shared_ptr<Player>& playerToAct, ActionType clientAction, vector<PossibleAction>& possibleActions, size_t bigBlind, size_t initialChips, size_t bigStackAmongOthers) {
    // Case 1: Check or Fold (Bet Amount is 0)
    if (clientAction == ActionType::CHECK || clientAction == ActionType::FOLD) {
        return 0;
    }

    size_t maxBet = min(initialChips, bigStackAmongOthers);
    // cout << "initial, bigStackOthers, maxBet is: " << initialChips << bigStackAmongOthers << maxBet << endl;

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
        size_t standardRaiseSize = 2 * prevBetAmount;

        // Edge case where a player is all in to raise (no choice)
        if (standardRaiseSize >= maxBet) return maxBet;
        else {
            minBet = standardRaiseSize;
        }
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
            if (isValidAmount(amount, minBet, maxBet)) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return amount;
            }
        }
    }
}

// Helper Functions

void ClientManager::displayPossibleActions(shared_ptr<Player>& playerToAct, vector<PossibleAction>& possibleActions) {
    cout << "Displaying possible actions for " << playerToAct->getName() << ":" << endl;
    bool isBigBlind = false;
    if (playerToAct->getPosition() == Position::BIG_BLIND) isBigBlind = true;
    ActionManager::displayPossibleActions(possibleActions, isBigBlind);
}

void ClientManager::displayClientAction(const ClientAction& clientAction) {
    cout << "Displaying ClientAction object attributes:" << endl;
    cout << "   Player: " << clientAction.player->getName() << endl;
    cout << "   Action Type: " << Action::actionTypeToStr(clientAction.type) << endl;
    cout << "   Amount: " << clientAction.amount << endl;
}

ActionType ClientManager::strToActionType(string& str, bool isBigBlind) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);

    if (str == "bet") return ActionType::BET;
    else if (str == "check" && !isBigBlind) return ActionType::CHECK;
    else if (str == "check" && isBigBlind) return ActionType::CALL;
    else if (str == "fold") return ActionType::FOLD;
    else if (str == "call") return ActionType::CALL;
    else if (str == "raise") return ActionType::RAISE;
    else if (str == "blind") return ActionType::BLIND;
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