#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "ActionManager.h"
#include "Dealer.h"
#include "ClientManager.h"
#include "GamePlayers.h"
#include "PotManager.h"
#include "TurnManager.h"

#include <memory.h>
using namespace std;

class GameController {
private:
    shared_ptr<GamePlayers> gamePlayers;
    shared_ptr<Dealer> dealer;

    shared_ptr<ActionManager> actionManager;
    shared_ptr<TurnManager> turnManager;
    shared_ptr<ClientManager> clientManager;
    shared_ptr<PotManager> potManager;
    shared_ptr<TurnManager> turnManager;
public:
    GameController();

    // Methods

    // runGameLoop: Manages overall game flow

    // startNewRound: Starts a new round of poker and resets game state

    // dealPlayers: Deals cards to players

    // dealCommunity: Deals community cards

    // getPlayerToAct: Fetches the current player to act

    // requestClientAction: Fetches action for the current player

    // processAction: Add action to action timeline

    // updatePotManager: Update the pot after each player action

    // updateTurnManager: Update active / folded players after each player action

    // advanceToNextStreet: In a given round, advances to the next street

    // showdown: Evaluates the winner and distributes the pot

    // rotatePositions: Update player position after round completion

    // isStreetOver: Checks if the current betting street is complete

    // isGameOver: Checks if the game is over (e.g. less than 2 players)

    // To add methods.
};

#endif // GAME_CONTROLLER