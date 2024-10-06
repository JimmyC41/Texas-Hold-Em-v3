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
        player = std::make_shared<Player>("Jimmy", Position::SMALL_BLIND, 1000);
    }
};

TEST_F(ClientManagerTest, CheckOption) {
    std::vector<PossibleAction> possibleActions = {{CHECK, 0}, {BET, 0}, {FOLD, 0}};
    ClientAction action = clientManager.getClientAction(player, possibleActions);
}

TEST_F(ClientManagerTest, Bet) {
    std::vector<PossibleAction> possibleActions = {{CHECK, 0}, {BET, 0}, {FOLD, 0}};
    ClientAction action = clientManager.getClientAction(player, possibleActions);
}

TEST_F(ClientManagerTest, BetAllIn) {
    std::vector<PossibleAction> possibleActions = {{CHECK, 0}, {BET, 0}, {FOLD, 0}};
    ClientAction action = clientManager.getClientAction(player, possibleActions);
}

TEST_F(ClientManagerTest, FoldOption) {
    std::vector<PossibleAction> possibleActions = {{CALL, 100}, {RAISE, 100}, {FOLD, 0}};
    ClientAction action = clientManager.getClientAction(player, possibleActions);
}

TEST_F(ClientManagerTest, CallOption) {
    std::vector<PossibleAction> possibleActions = {{CALL, 500}, {RAISE, 500}, {FOLD, 0}};
    ClientAction action = clientManager.getClientAction(player, possibleActions);
}

TEST_F(ClientManagerTest, CallAllIn) {
    std::vector<PossibleAction> possibleActions = {{CALL, 2000}, {RAISE, 2000}, {FOLD, 0}};
    ClientAction action = clientManager.getClientAction(player, possibleActions);
}

TEST_F(ClientManagerTest, RaiseOption) {
    std::vector<PossibleAction> possibleActions = {{CALL, 100}, {RAISE, 100}, {FOLD, 0}}; 
    ClientAction action = clientManager.getClientAction(player, possibleActions);
}

TEST_F(ClientManagerTest, RaiseAllIn) {
    std::vector<PossibleAction> possibleActions = {{CALL, 750}, {RAISE, 750}, {FOLD, 0}}; 
    ClientAction action = clientManager.getClientAction(player, possibleActions);
}