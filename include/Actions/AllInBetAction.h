#ifndef ALL_IN_BET_ACTION_H
#define ALL_IN_BET_ACTION_H

#include "../Action.h"

class AllInBetAction : public Action {
private:
    size_t amount;
public:
    AllInBetAction(shared_ptr<Player> player, size_t amount) : Action(player), amount(amount) {}
    
    ActionType getActionType() const override {
        return ALL_IN_BET;
    }

    size_t getAmount() const override {
        return amount;
    }
};

#endif // ALL_IN_BET_ACTION_H