#ifndef ALL_IN_CALL_H
#define ALL_IN_CALL_H

#include "../Action.h"

class AllInCallAction : public Action {
private:
    size_t amount;
public:
    AllInCallAction(shared_ptr<Player> player, size_t amount) : Action(player), amount(amount) {}
    
    ActionType getActionType() const override {
        return ALL_IN_CALL;
    }

    size_t getAmount() const override {
        return amount;
    }
};

#endif // ALL_IN_CALL_H