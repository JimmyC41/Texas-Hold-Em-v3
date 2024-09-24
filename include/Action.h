#ifndef ACTION_H
#define ACTION_H

#include "Player.h"

enum ActionType {
    CHECK,
    BET,
    CALL,
    RAISE,
    FOLD
};

class Action {
private:
    shared_ptr<Player> player;
public:
    Action(shared_ptr<Player> player): player(player) {}
    virtual ~Action() {}
    virtual ActionType getActionType() const = 0;
};

#endif // ACTION_H