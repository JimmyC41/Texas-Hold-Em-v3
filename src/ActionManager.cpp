#include "../include/ActionManager.h"
#include "../include/Action.h"
#include <iostream>
using namespace std;

ActionManager::ActionManager() : actionTimeline(), actionState(), activeBet(0) {}

void ActionManager::clearActionTimeline() {
    actionTimeline.clear();
    resetActionState();
    activeBet = 0;
}

void ActionManager::addActionToTimeline(shared_ptr<Action> action) {
    actionTimeline.push_back(action);
    updateActionState(action);

    ActionType actiontype = action->getActionType();
    if (actiontype == BLIND || actiontype == BET || actiontype == RAISE || actiontype == ALL_IN_BET) {
        // Update the active bet if the bet/raise is greater than the current bet to be matched
        if (action->getAmount() > activeBet) {
            activeBet = action->getAmount();
        }
    }
}

vector<PossibleAction> ActionManager::getAllowedActionTypes(bool playerCanRaise) {
    // No player has acted. Player to act can check or bet.
    if (actionTimeline.empty()) return {{CHECK, 0}, {BET, activeBet}, {FOLD, 0}};

    // Get the most recent action that is not a call or fold.
    ActionType lastAction = getLastAction();

    if (lastAction == CHECK) {
        return {{CHECK, 0}, {BET, activeBet}, {FOLD, 0}};
    } else if (lastAction == BLIND || lastAction == BET || lastAction == RAISE || lastAction == ALL_IN_BET || lastAction == ALL_IN_CALL) {
        if (playerCanRaise) {
            return {{CALL, activeBet}, {RAISE, activeBet}, {FOLD, 0}};
        } else {
            return {{CALL, activeBet}, {FOLD, 0}};
        }
    } else {
        // All players before have voluntarily folded.
        return {{CHECK, 0}, {BET, activeBet}, {FOLD, 0}};
    }
}

bool ActionManager::isActionsFinished(int numPlayers) const {
    printActionState();
    
    int checks = actionState.getChecks();
    int calls = actionState.getCalls();
    int sittingOut = actionState.getSittingOut();
    int folded = actionState.getFolded();

    if (checks == numPlayers) return true;

    if (actionState.isLimpedPreFlop()) return calls == (numPlayers - sittingOut - folded);
    else return calls == (numPlayers - sittingOut - folded - 1);

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

void ActionManager::updateActionState(shared_ptr<Action> action) {
    ActionType type = action->getActionType();

    // When we encounter an all-in bet, all other players that were previously
    // all-in are effectively sitting out of the betting action.
    // There are TWO implications that follow for all-in bets:
    // 1) numSittingOut is set BEFORE incrementing the all-in bet counter
    // We don't care about the most recent all-in bet because that acts as the 'agressor'
    // 2) numSittingOut is only updated if there is at least 2 all-in bets!
    // If there is only 1, it just acts as a normal bet and there is no need to adjust
    // for 1 fewer call!

    switch(type) {
        case BET:
        case RAISE:
            actionState.clearCalls();
            actionState.setNumSittingOut();
            actionState.setLimpAroundFalse();
            break;
        case ALL_IN_BET:
            actionState.clearCalls(); 
            actionState.setNumSittingOut();
            actionState.incrementAllInBets();
            actionState.setLimpAroundFalse();
            break;
        case BLIND:
            actionState.clearCalls();
        case CALL:
            actionState.incrementCalls();
            break;
        case ALL_IN_CALL:
            actionState.incrementAllInCalls();
            actionState.incrementSittingOut();
            break;
        case FOLD:
            actionState.incrementFolded();
            break;
        case CHECK:
            actionState.incrementChecks();
            actionState.setLimpAroundFalse();
            break;
        default:
            break;
    }
}

void ActionManager::resetActionState() {
    actionState.numCalls = 0;
    actionState.numChecks = 0;
    actionState.numFolded = 0;
    actionState.numAllInBet = 0;
    actionState.numAllInCall = 0;
    actionState.numSittingOut = 0;
    actionState.limpAround = true;
}

void ActionManager::displayPossibleActions(vector<PossibleAction>& actions, bool isBigBlind) {
    for (size_t i = 0; i < actions.size(); ++i) {
        PossibleAction action = actions[i];

        if (action.type == CHECK) {
            cout << "   Option: Check" << endl;
        } else if (action.type == BET) {
            cout << "   Option: Bet" << endl;
        } else if (action.type == CALL) {
            if (isBigBlind) {
                cout << "   Option: Check (amount: " << action.amount << ")" << endl;
            } else {
                cout << "   Option: Call (amount: " << action.amount << ")" << endl;
            }
        } else if (action.type == RAISE) {
            cout << "   Option: Raise (amount must exceed the current bet of: " << action.amount << ")" << endl;
        } else if (action.type == FOLD) {
            cout << "   Option: Fold" << endl;
        }
    }
}

void ActionManager::printActionState() const {
    cout << "===== Action State =====" << endl;
    cout << "Number of Calls: " << actionState.getCalls() << endl;
    cout << "Number of Checks: " << actionState.getChecks() << endl;
    cout << "Number of Folded Players: " << actionState.getFolded() << endl;
    cout << "Number of All-In Bets: " << actionState.numAllInBet << endl;
    cout << "Number of All-In Calls: " << actionState.numAllInCall << endl;
    cout << "Total Sitting Out: " << actionState.getSittingOut() << endl;
    cout << "Is Limped Around: " << (actionState.isLimpedPreFlop() ? "Yes" : "No") << endl;
    cout << "========================" << endl;
}