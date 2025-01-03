#include "../include/ClientManager.h"
#include "../include/ActionManager.h"
#include "../include/TurnManager.h"
#include "../include/Action.h"
#include <algorithm>

ClientManager::ClientManager(size_t bigBlind) : bigBlind(bigBlind) {}

ClientAction ClientManager::getClientAction(const StreetState& streetState, vector<PossibleAction>& possibleActions) {
    // If betting street is preflop and the player to act is big blind, the 'check' is a call of the active bet.
    // This must be reflected when displaying possible actions, and also fetching of the action type.
    displayPossibleActions(streetState, possibleActions);

    // Fetch action type from client
    ActionType clientActionType = getClientActionType(streetState, possibleActions);

    // Fetch bet amount from client
    size_t amount = getClientBetAmount(streetState, possibleActions, clientActionType);

    cout << "BET AMOUNT IS " << amount << endl;

    ClientAction clientAction = ClientAction{streetState.getCurPlayer(), clientActionType, amount};
    // displayClientAction(clientAction);

    cout << "\n" << endl;
    return clientAction;
}

// Client Helper Function

ActionType ClientManager::getClientActionType(const StreetState& streetState, vector<PossibleAction>& possibleActions) {
    string actionStr;
    ActionType actionType;

    while (true) {
        cout << "Please enter a valid action: ";
        getline(cin, actionStr);

        actionType = strToActionType(actionStr, streetState.isPlayerBigBlindPreFlop());
        if (isValidAction(possibleActions, actionType)) break;
    }
    return actionType;
}

size_t ClientManager::getClientBetAmount(const StreetState& streetState, vector<PossibleAction>& possibleActions, ActionType clientAction) {
    // Case 1: Check or Fold (Bet Amount is 0)
    if (clientAction == ActionType::CHECK || clientAction == ActionType::FOLD) {
        return 0;
    }

    // maxBet is the maximum amount the client can 'bet' provided how many chips they have, and the stack of others
    // If the player is the big stack among the table, the max they can bet is the next biggest stack
    // If everyone else has gone all-in or folded, by definition, the player to act must be at least the biggest stack,
    // so they just end up calling the active bet, and the callAmount will never be greater than their initial chips!
    size_t bigStackAmongOthers = streetState.getBigStackAmongOthers();
    size_t initialChips = streetState.getPlayerInitialChips();
    size_t maxBet = (bigStackAmongOthers == 0) ? initialChips : min(initialChips, bigStackAmongOthers);
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

void ClientManager::displayPossibleActions(const StreetState& streetState, vector<PossibleAction>& possibleActions) {
    cout << "Displaying possible actions for " << streetState.getCurPlayer()->getName() << ":" << endl;
    ActionManager::displayPossibleActions(possibleActions, streetState.isPlayerBigBlindPreFlop());
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
    else if (str == "check" && isBigBlind) return ActionType::CALL;
    else if (str == "check" && !isBigBlind) return ActionType::CHECK;
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