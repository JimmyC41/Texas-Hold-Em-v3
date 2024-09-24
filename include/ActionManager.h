#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H

#include "Action.h"
#include <vector>
using namespace std;

class ActionManager {
private:
    // Ordered list of betting actions in a given betting street
    vector<shared_ptr<Action>> actionTimeline;

    // Current bet to call/raise
    size_t activeBet;

    // Helper function to fetch the most recent action in the
    // actionTimeline that is not of type CALL or FOLD.
    ActionType getLastAction() const;

public:
    ActionManager();

    // Clear action timeline. Called at the END of each street.
    void clearActionTimeline();

    // Resets current bet. Called at the END of each street.
    void resetActiveBet();

    // Adds an Action object to the actionTimeline.
    // Sets the active bet.
    // Called when a player action is recorded.
    void addActionToTimeline(shared_ptr<Action> action);

    // Retrieves possible action types given the betting action. Called when player is prompted for their action.
    vector<ActionType> getAllowedActionTypes();
};

#endif // ACTION_MANAGER_H