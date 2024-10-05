#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Action.h"
#include "ActionManager.h"
#include "Board.h"
#include "Card.h"
#include "Dealer.h"
#include "ClientManager.h"
#include "Deck.h"
#include "GamePlayers.h"
#include "Player.h"
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
    
    void startNewRound();
    void advanceStreet();
    void showdown();
    // To add methods.
};

#endif // GAME_CONTROLLER