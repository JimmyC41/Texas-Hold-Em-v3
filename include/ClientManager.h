#ifndef CLIENT_H
#define CLIENT_H

#include "Action.h"
#include "ActionManager.h"
#include "TurnManager.h"
#include "StreetState.h"
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

typedef struct ClientAction {
    shared_ptr<Player> player;
    ActionType type;
    size_t amount;
} ClientAction;

class ClientManager {
public:
    ClientManager(size_t bigBlind);

    // Queries the client for a valid client action object to be processed by the action manager
    ClientAction getClientAction(const StreetState& streetState, vector<PossibleAction>& possibleActions);
private:
    size_t bigBlind;

    // Print possible actions for the player to act
    void displayPossibleActions(const StreetState& streetState, vector<PossibleAction>& possibleActions);

    // Print a given client action object
    void displayClientAction(const ClientAction& clientAction);

    // Fetch the action type from the client (stdin)
    ActionType getClientActionType(const StreetState& streetState, vector<PossibleAction>& possibleActions);

    // Fetch the bet amount from the client (stdin)
    size_t getClientBetAmount(const StreetState& streetState, vector<PossibleAction>& possibleActions, ActionType clientAction);

    // Convert string (client input) into an ActionType
    ActionType strToActionType(string& string, bool isBigBlind);

    // Checks if the client chosen action is valid given a possible actions array
    bool isValidAction(vector<PossibleAction>& possibleActions, ActionType chosenAction);

    // Given a valid client action type, fetches the relevant bet size from the possible actions array
    // E.g. If client action type is CALL and the possible actions array contains {CALL, 50}, client calls 50
    size_t getRelevantBet(vector<PossibleAction>& possibleActions, ActionType chosenAction);

    // Checks if the client bet size chosen is valid
    bool isValidAmount(size_t amount, size_t min, size_t max);
};

#endif // CLIENT_H