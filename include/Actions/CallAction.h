#ifndef CALL_ACTION_H
#define CALL_ACTION_H

#include "../Action.h"

class CallAction : public Action {
private:
    size_t amount;
public:
    CallAction(shared_ptr<Player> player, size_t amount) : Action(player), amount(amount) {}
    
    ActionType getActionType() const override {
        return CALL;
    }

    size_t getAmount() const override {
        return amount;
    }
};

#endif // CALL_ACTION_H