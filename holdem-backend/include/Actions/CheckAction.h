#ifndef CHECK_ACTION_H
#define CHECK_ACTION_H

#include "../Action.h"

class CheckAction : public Action {
public:
    CheckAction(shared_ptr<Player> player) : Action(player) {}
    
    ActionType getActionType() const override {
        return CHECK;
    }
    
    size_t getAmount() const override {
        return 0;
    }
};

#endif // CHECK_ACTION_H