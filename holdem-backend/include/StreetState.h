#ifndef STREET_STATE_H
#define STREET_STATE_H

#include "Player.h"
#include "Action.h"
#include <memory.h>

enum Street {
    PRE_FLOP = 0,
    FLOP,
    TURN,
    RIVER,
    SHOWDOWN
};

// StreetState is initalised with dummy values
typedef struct StreetState {
    Street street = Street::PRE_FLOP;
    int initialNumPlayersInHand = 0;
    shared_ptr<Player> curPlayer;
    bool isBigBlindPreFlop = false;
    shared_ptr<Action> playerAction;
    bool playerCanRaise = true;
    size_t activeBet = 0;
    size_t playerInitialChips = 0;
    size_t bigStackAmongOthers = 0;

    // Setter Methods (called BEFORE fetching current player)
    // Called in the setupStreet method
    void setStreet(Street newStreet) { street = newStreet; }
    void setInitialNumPlayers(int n) { initialNumPlayersInHand = n; }

    // Setter Methods (called AFTER fetching current player)
    // Called in the updateStreetStateForCurPlayer methods
    void setCurPlayer(const shared_ptr<Player>& player) { curPlayer = player; }
    void setActiveBet(size_t bet) { activeBet = bet; }
    void setPlayerInitialChips(size_t chips) { playerInitialChips = chips; }
    void setBigStackAmongOthers(size_t chips) { bigStackAmongOthers = chips; }
    void setPlayerCanRaise(bool canRaise) { playerCanRaise = canRaise; }
    void setIsBigBlindPreFlop(bool isBB) { isBigBlindPreFlop = isBB; }

    // Getter Methods
    Street getStreet() const { return street; }
    const shared_ptr<Player>& getCurPlayer() const { return curPlayer; }
    int getInitialNumPlayers() const { return initialNumPlayersInHand; }
    size_t getActiveBet() const { return activeBet; }
    size_t getPlayerInitialChips() const { return playerInitialChips; }
    size_t getBigStackAmongOthers() const { return bigStackAmongOthers; }
    bool getPlayerCanRaise() const { return playerCanRaise; }
    bool isPlayerBigBlindPreFlop() const { return isBigBlindPreFlop; }

    void resetStreetState() {
        street = Street::PRE_FLOP;
        initialNumPlayersInHand = 0;
        shared_ptr<Player> curPlayer = nullptr;
        isBigBlindPreFlop = false;
        playerAction = nullptr;
        playerCanRaise = true;
        activeBet = 0;
        playerInitialChips = 0;
        bigStackAmongOthers = 0;
    }
} StreetState;

#endif // STREET_STATE