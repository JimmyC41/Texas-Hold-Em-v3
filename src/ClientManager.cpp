#include "../include/ClientManager.h"
#include "../include/ActionManager.h"
#include "../include/Action.h"

ClientManager::ClientManager(size_t bigBlind) : bigBlind(bigBlind) {}

ClientAction ClientManager::getClientAction(shared_ptr<Player>& playerToAct, vector<PossibleAction>& possibleActions) {
    // Display possible actions for the player to act
    cout << "Displaying possible actions for" << playerToAct->getName() << ":" << endl;
    ActionManager::displayPossibleActions(possibleActions);

    // Ask for client input
    string actionStr;
    ActionType actionType;
    size_t betSize = 0; // Bet amount is only updated if the action is call, bet or raise

    // Fetch the action type
    while (true) {
        cout << "Please enter a valid action: " << endl;
        getline(cin, actionStr);
        actionType = strToActionType(actionStr);
        if (isValidAction(possibleActions, actionType)) break;
    }

    // Fetch the bet size (if needed)
    if (actionType == ActionType::CALL) {
        size_t max = playerToAct->getChips();
        betSize = getRelevantBet(possibleActions, actionType);
        if (betSize > max) betSize = max;
    } else if (actionType == ActionType::BET || RAISE) {
        size_t min = bigBlind; // By default, minimum bet is big blind
        size_t max = playerToAct->getChips();

        // Update min if there is an outstanding bet
        if (actionType == ActionType::RAISE) min = getRelevantBet(possibleActions, actionType);

        while (true) {
            cout << "Please enter a bet size of [" << min << ", " << max << "]" << endl;
            cin >> betSize;
            if (isValidAmount(betSize, min, max)) break;
        }
    }

    cout << "Displaying ClientAction attributes:" << endl;
    cout << "   Player: " << playerToAct->getName() << endl;
    cout << "   Action Type: " << Action::actionTypeToStr(actionType) << endl;
    cout << "   Bet Amount: " << betSize << endl;

    return ClientAction{playerToAct, actionType, betSize};
}

// Helper Functions

ActionType ClientManager::strToActionType(string& str) {
    if (str == "bet") return ActionType::BET;
    if (str == "check") return ActionType::BET;
    if (str == "fold") return ActionType::FOLD;
    if (str == "call") return ActionType::CALL;
    if (str == "raise") return ActionType::RAISE;
    if (str == "blind") return ActionType::BLIND;
}

bool ClientManager::isValidAction(vector<PossibleAction>& possibleActions, ActionType chosenAction) {
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