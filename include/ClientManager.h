#ifndef CLIENT_H
#define CLIENT_H

#include "Action.h"
#include "ActionManager.h"
#include <iostream>
#include <vector>
#include <memory>
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
    ClientAction getClientAction(shared_ptr<Player>& playerToAct, vector<PossibleAction>& possibleActions);
private:
    size_t bigBlind;

    // Print possible actions for the player to act
    void displayPossibleActions(shared_ptr<Player>& playerToAct, vector<PossibleAction>& possibleActions);

    // Print a given client action object
    void displayClientAction(const ClientAction& clientAction);

    // Fetch the action type from the client (stdin)
    ActionType getClientActionType(vector<PossibleAction>& possibleActions);

    // Fetch the bet amount from the client (stdin)
    size_t getClientBetAmount(shared_ptr<Player>& playerToAct, ActionType clientAction, 
                                vector<PossibleAction>& possibleActions, size_t bigBlind);

    // Convert string (client input) into an ActionType
    ActionType strToActionType(string& string);

    // Checks if the client chosen action is valid given a possible actions array
    bool isValidAction(vector<PossibleAction>& possibleActions, ActionType chosenAction);

    // Given a valid client action type, fetches the relevant bet size from the possible actions array
    // E.g. If client action type is CALL and the possible actions array contains {CALL, 50}, client calls 50
    size_t getRelevantBet(vector<PossibleAction>& possibleActions, ActionType chosenAction);

    // Checks if the client bet size chosen is valid
    bool isValidAmount(size_t amount, size_t min, size_t max);
};

#endif // CLIENT_H