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