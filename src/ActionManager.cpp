#include "../include/ActionManager.h"
#include "../include/Action.h"
#include <iostream>
using namespace std;

ActionManager::ActionManager() : actionTimeline(), activeBet(0) {}

void ActionManager::clearActionTimeline() {
    actionTimeline.clear();
    activeBet = 0;
}

void ActionManager::addActionToTimeline(shared_ptr<Action> action) {
    actionTimeline.push_back(action);

    ActionType actiontype = action->getActionType();

    if (actiontype == BLIND || actiontype == BET || actiontype == RAISE) {

        // Update the active bet if the bet/raise is greater than the current bet to be matched
        if (action->getAmount() > activeBet) {
            activeBet = action->getAmount();
        }
    }
}

vector<PossibleAction> ActionManager::getAllowedActionTypes() {
    // No player has acted. Player to act can check or bet.
    if (actionTimeline.empty()) return {{CHECK, 0}, {BET, activeBet}, {FOLD, 0}};

    // Get the most recent action that is not a call or fold.
    ActionType lastAction = getLastAction();

    if (lastAction == CHECK) {
        return {{CHECK, 0}, {BET, activeBet}, {FOLD, 0}};
    }
    else if (lastAction == BLIND || lastAction == BET || lastAction == RAISE) {
        return {{CALL, activeBet}, {RAISE, activeBet}, {FOLD, 0}};
    }
    else {
        // All players before have voluntarily folded.
        return {{CHECK, 0}, {BET, activeBet}, {FOLD, 0}};
    }
}

bool ActionManager::isActionsFinished(int numPlayers) const {
    // If there are n players and a player initiates a bet/raise, n-1 players must call/fold
    int numCallsRequired = numPlayers - 1;
    int numCalls = 0;

    int numChecksRequired = numPlayers;
    int numChecks = 0;

    // Iterate through the action timeline
    for (const auto& actionPtr : actionTimeline) {
        ActionType actionType = actionPtr->getActionType();

        // If we encounter a 'new' active bet, reset the number of calls
        if (actionType == BLIND || actionType == BET || actionType == RAISE) {
            numCalls = 0;
        }

        // If a player calls an active bet, increment the number of calls
        else if (actionType == CALL) {
            numCalls++;
        }

        // If a player folds, decrement number of calls required
        else if (actionType == FOLD) {
            numChecksRequired--;
            numCallsRequired--;
        }

        // If a player checks, increment number of checks
        else if (actionType == CHECK) {
            numChecks++;
        }

        // Betting is over when all players have called or folded to the active bet
        // or when all players have checked
        if (numCalls == numCallsRequired || numChecks == numChecksRequired) {
            return true;
        }
    }

    return false;
}

void ActionManager::displayActionTimeline() {
    cout << "Displaying the action timeline. Active bet is: " << activeBet << "!" << endl;

    for (auto& it : actionTimeline) {
        cout << "Player: " << it->getPlayerName() << " | Action: " 
            << Action::actionTypeToStr(it->getActionType()) << endl;
    }
}

size_t ActionManager::getNumActions() const {
    return actionTimeline.size();
}

size_t ActionManager::getActiveBet() const {
    return activeBet;
}

// Helper Functions

ActionType ActionManager::getLastAction() const {
    for (auto it = actionTimeline.rbegin(); it != actionTimeline.rend(); ++it) {
        ActionType actionType = (*it)->getActionType();
        if (actionType != CALL && actionType != FOLD) {
            return actionType;
        }
    }
    throw runtime_error("No valid action found!");
}

void ActionManager::displayPossibleActions(vector<PossibleAction>& actions) {
    for (size_t i = 0; i < actions.size(); ++i) {
        PossibleAction action = actions[i];

        if (action.type == CHECK) {
            cout << "   Option: Check" << endl;
        } else if (action.type == BET) {
            cout << "   Option: Bet" << endl;
        } else if (action.type == CALL) {
            cout << "   Option: Call (amount: " << action.amount << ")" << endl;
        } else if (action.type == RAISE) {
            cout << "   Option: Raise (amount must exceed the current bet of: " << action.amount << ")" << endl;
        } else if (action.type == FOLD) {
            cout << "   Option: Fold" << endl;
        }
    }
}