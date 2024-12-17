#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H

#include "Action.h"
#include <vector>
using namespace std;


typedef struct PossibleAction {
    ActionType type;
    size_t amount;

    bool operator==(const PossibleAction& other) const {
        return type == other.type && amount == other.amount;
    }
} PossibleAction;

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

    // Clear action timeline and set active bet to 0.
    // Called at the END of each street.
    void clearActionTimeline();

    // Adds an Action object to the actionTimeline.
    // Sets the active bet.
    // Called when a player action is recorded.
    void addActionToTimeline(shared_ptr<Action> action);

    // Retrieves possible action types given the betting action.
    // Called when player is prompted for their action.
    vector<PossibleAction> getAllowedActionTypes(bool playerCanRaise);

    // Iterates through the action timeline and checks if betting action is complete
    bool isActionsFinished(int numPlayers) const;

    // Displays the action types in the action timeline.
    void displayActionTimeline();

    // Returns the size of the action timeline
    size_t getNumActions() const;

    // Returns the active bet
    size_t getActiveBet() const;

    // Helper function to display a vector of possible actions
    static void displayPossibleActions(vector<PossibleAction>& actions, bool isBigBlind);
};

#endif // ACTION_MANAGER_H