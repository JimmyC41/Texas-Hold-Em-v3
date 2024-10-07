#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "ActionManager.h"
#include "Action.h"
#include "Actions/BetAction.h"
#include "Actions/BlindAction.h"
#include "Actions/CallAction.h"
#include "Actions/CheckAction.h"
#include "Actions/FoldAction.h"
#include "Actions/RaiseAction.h"
#include "Dealer.h"
#include "ClientManager.h"
#include "GamePlayers.h"
#include "PotManager.h"
#include "TurnManager.h"
#include "Deck.h"
#include "Board.h"

#include <memory.h>
using namespace std;

enum Street {
    PRE_FLOP = 0,
    FLOP,
    TURN,
    RIVER,
    SHOWDOWN
};

class GameController {
private:
    size_t smallBlind;
    size_t bigBlind;

    Deck deck;
    Board board;
    Dealer dealer;
    GamePlayers gamePlayers;
    ActionManager actionManager;
    TurnManager turnManager;
    ClientManager clientManager;
    PotManager potManager;
public:
    GameController(size_t smallBlind, size_t bigBlind);

    // GAME STATE INFORMATION METHODS

    int getNumPlayersInGame() const;

    // Displays names and chips for players in the game
    void displayPlayersInGame() const;

    // PLAYER SPECIFIC METHODS
    
    // Adds a new player to game players and turn manager
    void addNewPlayer(const string& name, size_t chips);

    // Removes an existing player from the game players and turn manager
    void removePlayer(const string& name);

    // DEALING SPECIFIC METHODS

    // Deals hole cards to players
    void dealPlayers();

    // Deals community cards to board
    void dealBoard(int numCards);

    // STREET SPECIFIC METHODS

    // Creates an action object from a client object
    shared_ptr<Action> createAction(const ClientAction& clientAction);
    
    // Check if all players have acted in a given round
    bool isStreetOver();

    // Sets the first player to act, handles blinds and deals players / board
    // Called at the beginning of each street
    void setupStreet(Street newStreet);

    // Start a new betting street
    void startStreet(Street newStreet);

    // ROUND METHODS

    // OVERALL GAME METHODS

    // END OF RD
    // TurnManager: Move folded players, rotate positions

    // runGameLoop: Manages overall game flow
    // advanceToNextStreet: In a given round, advances to the next street
    // showdown: Evaluates the winner and distributes the pot
    // isGameOver: Checks if the game is over (e.g. less than 2 players)
    // To add methods.
};

#endif // GAME_CONTROLLER