#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include "../include/ClientManager.h"
#include "../include/ActionManager.h"
#include "../include/Action.h"

// NOTE: Std is entered by user in terminal, could not figure out how to hard code input into tests

class ClientManagerTest : public ::testing::Test {
protected:
    ClientManager clientManager;
    std::shared_ptr<Player> player;

    ClientManagerTest() : clientManager(3) {
        player = std::make_shared<Player>("Player", Position::SMALL_BLIND, 1000);
    }
};

TEST_F(ClientManagerTest, CheckOption) {
    std::vector<PossibleAction> possibleActions = {{CHECK, 0}, {BET, 0}, {FOLD, 0}};

    // Check

    ClientAction action = clientManager.getClientAction(player, possibleActions);
    ASSERT_EQ(action.type, ActionType::CHECK);
}

TEST_F(ClientManagerTest, BetOption) {
    std::vector<PossibleAction> possibleActions = {{CHECK, 0}, {BET, 0}, {FOLD, 0}};

    // Bet 100

    ClientAction action = clientManager.getClientAction(player, possibleActions);
    ASSERT_EQ(action.type, ActionType::BET);
    ASSERT_EQ(action.amount, 100);
}

TEST_F(ClientManagerTest, BetAllIn) {
    std::vector<PossibleAction> possibleActions = {{CHECK, 0}, {BET, 0}, {FOLD, 0}};

    // Bet 1000

    ClientAction action = clientManager.getClientAction(player, possibleActions);
    ASSERT_EQ(action.type, ActionType::BET);
    ASSERT_EQ(action.amount, 1000);  // Assuming player's total chips are 1000
}

TEST_F(ClientManagerTest, FoldOption) {
    std::vector<PossibleAction> possibleActions = {{CALL, 100}, {RAISE, 100}, {FOLD, 0}};

    // Fold

    ClientAction action = clientManager.getClientAction(player, possibleActions);
    ASSERT_EQ(action.type, ActionType::FOLD);
}

TEST_F(ClientManagerTest, CallOption) {
    std::vector<PossibleAction> possibleActions = {{CALL, 100}, {RAISE, 100}, {FOLD, 0}};

    // Call

    ClientAction action = clientManager.getClientAction(player, possibleActions);
    ASSERT_EQ(action.type, ActionType::CALL);
    ASSERT_EQ(action.amount, 100);
}

TEST_F(ClientManagerTest, CallAllIn) {
    std::vector<PossibleAction> possibleActions = {{CALL, 1500}, {RAISE, 1500}, {FOLD, 0}};

    // Call

    ClientAction action = clientManager.getClientAction(player, possibleActions);
    ASSERT_EQ(action.type, ActionType::CALL);
    ASSERT_EQ(action.amount, 1000);
}

TEST_F(ClientManagerTest, RaiseOption) {
    std::vector<PossibleAction> possibleActions = {{CALL, 100}, {RAISE, 100}, {FOLD, 0}}; 

    // Raise 200

    ClientAction action = clientManager.getClientAction(player, possibleActions);
    ASSERT_EQ(action.type, ActionType::RAISE);
    ASSERT_EQ(action.amount, 200);
}