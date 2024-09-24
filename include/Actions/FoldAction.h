#ifndef FOLD_ACTION_H
#define FOLD_ACTION_H

#include "../Action.h"

class FoldAction : public Action {
public:
    FoldAction(shared_ptr<Player> player) : Action(player) {}
    
    ActionType getActionType() const override {
        return FOLD;
    }
};

#endif // FOLD_ACTION_H