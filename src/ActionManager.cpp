#include "../include/ActionManager.h"
#include "../include/Action.h"

ActionManager::ActionManager() : actionTimeline(), activeBet(0) {}

void ActionManager::clearActionTimeline() {
    actionTimeline.clear();
}

void ActionManager::resetActiveBet() {
    activeBet = 0;
}

void ActionManager::addActionToTimeline(shared_ptr<Action> action) {
    actionTimeline.push_back(action);
}

vector<ActionType> ActionManager::getAllowedActionTypes() {
    // No player has acted. Player to act can check or bet.
    if (actionTimeline.empty()) return {CHECK, BET};

    // Get the most recent action that is not a call or fold.
    ActionType lastAction = getLastAction();

    if (lastAction == CHECK) {
        return {CHECK, BET};
    }
    else if (lastAction == BLIND || lastAction == BET || lastAction == RAISE) {
        return {CALL, RAISE};
    }
    else {
        // All players before have voluntarily folded.
        return {CHECK, BET};
    }
}

// Helper Functions

ActionType ActionManager::getLastAction() const {
    for (auto it = actionTimeline.rbegin(); it != actionTimeline.rend(); ++it) {
        ActionType actionType = (*it)->getActionType();
        if (actionType != CALL || actionType != FOLD) {
            return actionType;
        }
    }
    throw runtime_error("No valid action found!");
}