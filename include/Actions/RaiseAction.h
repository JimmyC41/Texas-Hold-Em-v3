#ifndef RAISE_ACTION_H
#define RAISE_ACTION_H

#include "../Action.h"

class RaiseAction : public Action {
private:
    size_t amount;
public:
    RaiseAction(shared_ptr<Player> player, size_t amount) : Action(player), amount(amount) {}
    
    ActionType getActionType() const override {
        return RAISE;
    }

    int getAmount() const {
        return amount;
    }
};

#endif // RAISE_ACTION_H