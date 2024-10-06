#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "ActionManager.h"
#include "Dealer.h"
#include "ClientManager.h"
#include "GamePlayers.h"
#include "PotManager.h"
#include "TurnManager.h"
#include "Deck.h"
#include "Board.h"

#include <memory.h>
using namespace std;

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

    // STREET METHOD

    // Check if all players have acted in a given round
    // TODO
    bool isStreetOver();

    // 

    // Set first player to act
    // Gets the current player to act
    // Get possible actions for the player to act
    // Request client action given possible actions
    // Add action to action timeline
    // Update the pot manager after each player action
    // Update active / folded players after each player action

    // END OF STREET
    // ActionManager: Clear action timeline
    // PotManager: Calculate pots and reset bets
    // TurnManager: Move folded players, rotate positions


    // runGameLoop: Manages overall game flow
    // Called after players are added

    // advanceToNextStreet: In a given round, advances to the next street
    // showdown: Evaluates the winner and distributes the pot
    // isGameOver: Checks if the game is over (e.g. less than 2 players)
    // To add methods.
};

#endif // GAME_CONTROLLER