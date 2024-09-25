#ifndef GAME_PLAYERS_H
#define GAME_PLAYERS_H

#include "Player.h"

#include <vector>
#include <memory>
using namespace std;

const int MAX_NUM_PLAYERS = 9;

class GamePlayers {
private:
    vector<shared_ptr<Player>> gamePlayers;

    // Helper function to check if there is an empty seat at a given position
    bool isEmptySeatInPosition(Position position) const;

    // Helper fuction to get the next free position
    Position getEmptySeatPosition() const;
public:
    GamePlayers();

    // Adds a player to the game and returns a pointer for other classes to use
    shared_ptr<Player> addPlayerToGame(const string& name, size_t chips);

    // Removes a player from the game
    void removePlayerFromGame(const string& playerName);

    // Removes all players from the game
    void removeAllPlayersInGame();

    // Checks if there is at least 2 players
    // Called BEFORE starting the game
    bool isEnoughPlayersInGame();
    
    // Returns the number of active players
    int getNumPlayersInGame() const;

    // Displays player names and chips
    void displayPlayersInGame() const;
};

#endif // GAME_PLAYERS_H