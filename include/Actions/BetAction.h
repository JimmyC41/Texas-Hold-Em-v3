#ifndef BET_ACTION_H
#define BET_ACTION_H

#include "../Action.h"

class BetAction : public Action {
private:
    size_t amount;
public:
    BetAction(shared_ptr<Player> player, size_t amount) : Action(player), amount(amount) {}
    
    ActionType getActionType() const override {
        return BET;
    }

    size_t getAmount() const override {
        return amount;
    }
};

#endif // BET_ACTION_H