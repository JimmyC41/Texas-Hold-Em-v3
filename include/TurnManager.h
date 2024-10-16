#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H

#include "Player.h"
#include <vector>
using namespace std;

class TurnManager {
private:
    // Contains players in the hand
    // If heads-up, must exist small and big blind.
    // If at least 3 players, must exist small blind, big blind and dealer.
    vector<shared_ptr<Player>> playersInHand;

    // Contains folded players that can not act
    vector<shared_ptr<Player>> playersFolded;

    shared_ptr<Player> playerWithButton;
    shared_ptr<Player> playerToAct;

    // Helper function to return a player with a given position (if the player exists)
    shared_ptr<Player> getPlayerWithPosition(Position position) const;

    // Helper fuction to return one position clockwise.
    Position getNextPosition(Position curPosition) const;

    // Helper function to sort players in hand and set button
    void sortPlayersInHand();

    // Helper function to set blinds and button
    void setBlindsAndButton();

    // Helper function to return the next player to act.
    shared_ptr<Player> getNextToAct();
public:
    TurnManager();

    // Adds a player to the playersInHand vector.
    // Sorts players, sets blinds and button.
    // Called for each player before starting a new round.
    void addPlayerInHand(const shared_ptr<Player>& player);

    // Moves a player from the playersInHand vector to the playersFolded vector.
    // Updates the current player to act as the previous player that acted in the in hand vector.
    // Sorts players in hand, but does not set blinds and button.
    // Called when a player has folded.
    void foldPlayerInHand(const shared_ptr<Player>& foldedPlayer);

    // Removes a player from the playersInHand vector.
    // Sorts player and sets blinds and button.
    // Called when a player leaves the game.
    void removePlayerFromHand(const shared_ptr<Player>& removedPlayer);

    // Moves players in folded vector back to the in hand vector.
    // Called at the END of the round.
    void moveFoldedPlayersToInHand();

    // Rotates players positions clockwise.
    // Called after folded players are moved to in hand.
    // Called at the END of the round
    void rotatePositions();

    // Sets the big blind as the player to act.
    // Useful for heads-up.
    // Called at the start of pre flop street.
    void setBigBlindToAct();

    // Sets the small blind as the player to act.
    // Called at the start of pre flop street IF heads up.
    void setSmallBlindToAct();

    // Sets the earliest position player as the player to act.
    // Called at the start of flop, turn and river.
    void setEarlyPositionToAct();

    // Returns the current player to act.
    // Updates current player to next player.
    // Called before each action is processed.
    shared_ptr<Player> getPlayerToAct();

    // Returns the player with the button and does NOT update playerToAct.
    shared_ptr<Player> getPlayerWithButton() const;

    // Return the number of players.
    int getNumPlayersInHand() const;

    // Return the number of players to act (excludes players all in)
    int getNumPlayersToAct() const;

    // Return the number of folded players.
    int getNumPlayersFolded() const;

    // Prints the current player to act
    void displayPlayerToAct() const;

    // Prints the player with the button
    void displayPlayerWithButton() const;

    // Prints the players in hand
    void displayPlayersInHand() const;

    // Fetches the vector of players in the hand
    vector<shared_ptr<Player>>& getPlayersInHand();
};

#endif // TURN_MANAGER_H