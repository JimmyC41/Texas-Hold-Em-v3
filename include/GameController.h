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

#include <string>
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
    int roundNum;

    Deck deck;
    Board board;
    Dealer dealer;
    GamePlayers gamePlayers;
    ActionManager actionManager;
    TurnManager turnManager;
    ClientManager clientManager;
    PotManager potManager;

    // Helper function to convert Street to string
    string streetToStr(Street street);

    // Helper function to deal hole cards to players
    void dealPlayers();

    // Helper function to deals community cards to board
    void dealBoard(int numCards);

    // Street Helper function to set up game state for a new street
    // Updates first player to act, handles blinds and deals players / board
    // Called at the beginning of each street
    void setupStreet(Street newStreet);

    // Street helper function to create an action object from a client object
    shared_ptr<Action> createAction(const ClientAction& clientAction);

    // Street helper function to check if players in the hand are all in
    bool isPlayersInHandAllIn();

    // Street helper function to check if there is only one player left (i.e. folded through)
    bool isFoldedThrough();

    // Street helper function to check if all players have acted in a given round
    bool isStreetOver(int initialPlayersInhand);

    // Round helper function to resets the game state before a new round
    // Called at the end of each round
    // TurnManager: Resets folded players and rotates posiitions
    // ActionManager: Clear the action timeline
    // PotManager: Reset recent bets and dead money
    void setupNewRound();

    // TODO Game helper function to check if all players have enough chips to play

    // Game helper function if there are at least two players in the game
    bool isEnoughPlayersInGame();

public:
    GameController(size_t smallBlind, size_t bigBlind);

    // Displays names and chips for players in the game
    void displayPlayersInGame() const;

    // TODO: Addition and removal of players needs to be abstracted to stdin/stdout

    // Helper function to add a new player to game players and turn manager
    void addNewPlayer(const string& name, size_t chips);

    // Helper function to remove an existing player from the game players and turn manager
    void removePlayer(const string& name);

    // Iniates a new betting street
    void startStreet(Street newStreet);

    // Iniates a new round of poker
    void startRound();

    // Main game method
    void main();
};

#endif // GAME_CONTROLLER