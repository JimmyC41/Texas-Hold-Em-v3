#ifndef ACTION_H
#define ACTION_H

#include "Player.h"
#include <iostream>
using namespace std;

enum ActionType {
    CHECK,
    BET,
    CALL,
    RAISE,
    FOLD,
    BLIND
};

class Action {
private:
    shared_ptr<Player> player;
public:
    Action(shared_ptr<Player> player): player(player) {}
    virtual ~Action() {}
    virtual ActionType getActionType() const = 0;
    virtual size_t getAmount() const = 0;

    string getPlayerName() {
        return player->getName();
    }

    static string actionTypeToStr(ActionType action) {
        switch (action) {
            case CHECK:
                return "Check";
            case BET:
                return "Bet";
            case CALL:
                return "Call";
            case RAISE:
                return "Raise";
            case FOLD:
                return "Fold";
            case BLIND:
                return "Blind";
            default:
                return "Unknown Action";
        }
    }
};

#endif // ACTION_H