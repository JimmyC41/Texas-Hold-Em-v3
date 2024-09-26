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
    void sortPlayersAndSetButton();
public:
    TurnManager();

    // Adds a player to the playersInHand vector.
    // Called for each player before starting a new round.
    void addPlayerInHand(const shared_ptr<Player>& player);

    // Moves a player from the playersInHand vector to the playersFolded vector.
    // Called when a player has folded.
    void foldPlayerInHand(const shared_ptr<Player>& foldedPlayer);

    // Moves players in folded vector back to the in hand vector.
    // Called at the start of the pre-flop round.
    void moveFoldedPlayersToInHand();

    // Rotates players positions clockwise.
    // Called at the start of the pre-flop round.
    void rotatePositions();

    // Returns the big blind to act and updates playerToAct.
    // Called at the start of pre flop street.
    shared_ptr<Player> getBigBlindToAct();

    // Returns the small blind to act and updates playerToAct.
    // Called at the start of pre flop street IF heads up.
    shared_ptr<Player> getSmallBlindToAct();

    // Returns the EP player to act and udpates playerToAct.
    // Called at the start of flop, turn and river.
    shared_ptr<Player>  getEarlyPositionToAct();

    // Returns the next player to act and updates playerToAct.
    // Called after each player action is processed.
    shared_ptr<Player> getNextToAct();

    // Returns the player with the button and does NOT update playerToAct.
    shared_ptr<Player> getPlayerWithButton() const;

    // Return the number of players.
    int getNumPlayersInHand() const;

    // Return the number of folded players.
    int getNumPlayersFolded() const;

    // Prints the current player to act
    void displayPlayerToAct() const;

    // Prints the player with the button
    void displayPlayerWithButton() const;

    // Prints the players in hand
    void displayPlayersInHand() const;
};

#endif // TURN_MANAGER_H