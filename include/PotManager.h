#ifndef POT_MANAGER_H
#define POT_MANAGER_H

#include "Player.h"
#include <set>
#include <map>
using namespace std;

typedef struct Pot {
    size_t chips;
    vector<shared_ptr<Player>> eligiblePlayers;

    Pot();
    void addPlayer(const shared_ptr<Player>& player);
    void addChips(size_t amount);
    size_t getChips() const;
    const vector<shared_ptr<Player>>& getEligiblePlayers() const;
    bool isPlayerInPot(const shared_ptr<Player>& player);
} Pot;

class PotManager {
private:
    // Vector of all pots
    vector<Pot> pots;

    // Maps players to their most recent bets in a betting street
    map<shared_ptr<Player>, size_t> playerBets;

    // Amount of dead chips from folded players.
    size_t deadChips;

    // Helper function to add a new side pot.
    void newPot();

    // Helper function to fetch the current pot.
    Pot& getCurPot();

    // Helper function to check if all bets have been placed in pots.
    bool allPotsCreated() const;

    // Helper function to find the minimum bet of all players.
    size_t findMinBet() const;

public:
    // Initalises a single pot.
    PotManager();

    // Updates the playerBets map after a bet/raises. Called after each player action.
    void addPlayerBet(const shared_ptr<Player>& player, size_t bet);

    // Gets a player's contribution to the pot in a given round
    size_t getRecentBet(const shared_ptr<Player>& player);

    // Increments dead money and sets folded player's recent bet to 0.
    void foldPlayerBet(const shared_ptr<Player>& player);

    // Calculates pots after betting action in a street is finished. Called after each street
    void calculatePots();

    // Resets recent bets and dead money to 0. Called after each street.
    void resetPlayerBets();

    void displayPlayerBets();
    void displayPots();
    const Pot& getPot(int index) const;
    int getNumPots() const;
};

#endif // POT_MANAGER_H