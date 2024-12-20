#ifndef POT_MANAGER_H
#define POT_MANAGER_H

#include "Player.h"
#include <set>
#include <map>
using namespace std;

struct BetInfo {
    size_t betSize;
    bool isAllIn;
};

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
    map<shared_ptr<Player>, BetInfo> playerBets;

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

    // Clears all pots for a new game. Called at the end of each round.
    void resetPots();

    // Updates the playerBets map after a bet/raises. Called after each player action.
    void addPlayerBet(const shared_ptr<Player>& player, size_t bet, bool isAllIn);

    // Gets a player's contribution to the pot in a given round
    size_t getRecentBet(const shared_ptr<Player>& player);

    // Increments dead money and sets folded player's recent bet to 0.
    void foldPlayerBet(const shared_ptr<Player>& player);

    // Calculates pots after betting action in a street is finished.
    // Called at the end of each round.
    void calculatePots();

    // Awards all pots to players according to a vector of players sorted by the strength of their hand.
    // Called AFTER calculating pots but BEFORE resetting player bets at the end of each round.
    void awardPots(vector<shared_ptr<Player>>& sortedPlayers);

    // Resets recent bets and dead money to 0. Called at the end of each round.
    void resetPlayerBets();

    // Gets the initial stack of specified player at the beginning of the round.
    size_t getInitialChips(const shared_ptr<Player>& player);

    // Gets the biggest stack of players in hand other than the specified player.
    // Used to calculate the maximum amount can bet (you can't bet more than someone can call!).
    size_t getBigStackAmongOthers(const shared_ptr<Player>& avoidPlayer, const vector<shared_ptr<Player>>& playersInHand);

    void displayPlayerBets();
    void displayPots();
    const Pot& getPot(int index) const;
    int getNumPots() const;
};

#endif // POT_MANAGER_H