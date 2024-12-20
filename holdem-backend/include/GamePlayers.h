#ifndef GAME_PLAYERS_H
#define GAME_PLAYERS_H

#include "Player.h"

#include <vector>
#include <memory>
using namespace std;

const int MIN_NUM_PLAYERS = 2;
const int MAX_NUM_PLAYERS = 9;

class GamePlayers {
private:
    vector<shared_ptr<Player>> gamePlayers;

    // Helper function to sort player order in the vector by their position
    void sortGamePlayers();

    // Helper function to check if there is an empty seat at a given position
    bool isEmptySeatInPosition(Position position) const;

    // Helper fuction to get the next free position
    Position getEmptySeatPosition() const;

    // Helper function to update blinds after player is removed
    void updateBlinds();

    // Helper function to get a player with a given position
    shared_ptr<Player> getPlayerWithPosition(Position position);

    // Helper function to get a player with a given name
    shared_ptr<Player> getPlayerWithName(const string& name);

    // Helper function to remove a player
    void removePlayer(shared_ptr<Player> playerToRemove);
public:
    GamePlayers();

    // Returns the vector of game players
    const vector<shared_ptr<Player>>& getGamePlayers() const;

    // Queries the client to enter player details for addition
    shared_ptr<Player> queryNewPlayer();

    // Queries the client if they wish to exit the game
    shared_ptr<Player> queryRemovePlayer();

    // Validate chip counts and query client for action (exit or add chips)
    void validateChipCounts(size_t minChipCount);

    // Adds a player to the game and returns a pointer for other classes to use
    // Sets last player added as the button
    // Called BEFORE rotating positions in turn manager
    shared_ptr<Player> addPlayerToGame(const string& name, size_t chips);

    // Removes a player from the game
    // Must be at least 3 players
    // Called BEFORE rotating positions in turn manager
    shared_ptr<Player> removePlayerFromGame(const string& playerName);

    // Removes all players from the game
    // Set last player to act after player removal added as the button
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