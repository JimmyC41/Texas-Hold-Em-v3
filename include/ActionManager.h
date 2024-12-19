#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H

#include "Action.h"
#include <vector>
using namespace std;

typedef struct ActionState {
    int numCalls = 0;
    int numChecks = 0;
    int numFolded = 0;
    int numAllInBet = 0;
    int numAllInCall = 0;
    int numSittingOut = 0; // Players that are all-in that cannot contribute to action
    bool limpAround = true;

    // Setter Methods
    void clearCalls() { numCalls = 0; }
    void setLimpAroundFalse() { limpAround = false; }
    void setNumSittingOut() {
        if (numAllInBet > 0) numSittingOut = (numAllInBet + numAllInCall);
        else numSittingOut = numAllInCall;
    }
    void incrementChecks() { numChecks++; }
    void incrementCalls() { numCalls++; }
    void incrementFolded() { numFolded++; }
    void incrementAllInBets() { numAllInBet++; }
    void incrementAllInCalls() { numAllInCall++; }
    void incrementSittingOut() { numSittingOut++; }

    // Getter Methods
    int getSittingOut() const { return numSittingOut; }
    int getChecks() const { return numChecks; }
    int getCalls() const { return numCalls; }
    int getFolded() const { return numFolded; }
    bool isLimpedPreFlop() const { return limpAround; }
} ActionState;

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

    // Struct to monitor the action state in a given betting street
    ActionState actionState;

    // Current bet to call/raise
    size_t activeBet;

    // Helper function to fetch the most recent action in the
    // actionTimeline that is not of type CALL or FOLD.
    ActionType getLastAction() const;

    // Helper function to update the action state given a new action
    void updateActionState(shared_ptr<Action> action);

    // Helper function to reset the action state
    void resetActionState();

public:
    ActionManager();

    // Clear action timeline and set active bet to 0.
    // Clears action state struct.
    // Called at the END of each street.
    void clearActionTimeline();

    // Adds an Action object to the actionTimeline and sets active bet.
    // Calls the update action state
    // Called when a player action is recorded.
    void addActionToTimeline(shared_ptr<Action> action);

    // Retrieves possible action types given the betting action.
    // Called when player is prompted for their action.
    vector<PossibleAction> getAllowedActionTypes(bool playerCanRaise);

    // Checks if betting action is complete given the current action state
    bool isActionsFinished(int numPlayers) const;

    // Displays the action types in the action timeline.
    void displayActionTimeline();

    // Helper function to print out action state for debugging
    void printActionState() const;

    // Returns the size of the action timeline
    size_t getNumActions() const;

    // Returns the active bet
    size_t getActiveBet() const;

    // Helper function to display a vector of possible actions
    static void displayPossibleActions(vector<PossibleAction>& actions, bool isBigBlind);
};

#endif // ACTION_MANAGER_H