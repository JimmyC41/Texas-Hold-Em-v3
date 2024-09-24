#ifndef BLIND_ACTION_H
#define BLIND_ACTION_H

#include "../Action.h"

class BlindAction : public Action {
private:
    size_t blindAmount;
public:
    BlindAction(shared_ptr<Player> player, size_t amount) : Action(player), blindAmount(amount) {}
    
    ActionType getActionType() const override {
        return BLIND;
    }

    int getAmount() const {
        return blindAmount;
    }
};

#endif // BLIND_ACTION_H