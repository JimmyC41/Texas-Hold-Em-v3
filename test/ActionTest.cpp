#include <gtest/gtest.h>
#include "../include/Player.h"
#include "../include/ActionManager.h"
#include "../include/Action.h"
#include "../include/Actions/BetAction.h"
#include "../include/Actions/BlindAction.h"
#include "../include/Actions/CallAction.h"
#include "../include/Actions/CheckAction.h"
#include "../include/Actions/FoldAction.h"
#include "../include/Actions/RaiseAction.h"

class ActionTest : public ::testing::Test {
protected:
    ActionManager actionManager;
    shared_ptr<Player> player1;
    shared_ptr<Player> player2;
    shared_ptr<Player> player3;
    shared_ptr<Player> player4;

    ActionTest() {
        player1 = make_shared<Player>("P1", Position::SMALL_BLIND, 2000);
        player2 = make_shared<Player>("P2", Position::BIG_BLIND, 10000);
        player3 = make_shared<Player>("P3", Position::UTG, 1500);
        player4 = make_shared<Player>("P4", Position::UTG_1, 6000);
    }

    void TearDown() override {
        actionManager.clearActionTimeline();
    }
};

TEST_F(ActionTest, AddingActionsToTimeline) {
    auto postSmall_1 = make_shared<BlindAction>(player1, 1);
    auto postBig_2 = make_shared<BlindAction>(player2, 2);
    auto callBig_3 = make_shared<CallAction>(player3, 2);
    auto callBig_4 = make_shared<CallAction>(player4, 2);
    auto callBig_1 = make_shared<CallAction>(player1, 2);
    auto check_2 = make_shared<CheckAction>(player2);

    actionManager.addActionToTimeline(postSmall_1);
    ASSERT_EQ(actionManager.getActiveBet(), 1);
    actionManager.addActionToTimeline(postBig_2);
    actionManager.addActionToTimeline(callBig_4);
    actionManager.addActionToTimeline(callBig_1);
    actionManager.addActionToTimeline(check_2);
    ASSERT_EQ(actionManager.getActiveBet(), 2);

    ASSERT_EQ(actionManager.getNumActions(), 5);

    actionManager.clearActionTimeline();
    ASSERT_EQ(actionManager.getNumActions(), 0);
}

TEST_F(ActionTest, AllowedActionsToNoAction) {
    ASSERT_EQ(actionManager.getActiveBet(), 0);
    vector<PossibleAction> actual = actionManager.getAllowedActionTypes();
    vector<PossibleAction> expected = {{CHECK, 0}, {BET, 0}, {FOLD, 0}};
    ASSERT_EQ(actual, expected);
}

TEST_F(ActionTest, AllowedActionsToCheck) {
    auto check_1 = make_shared<CheckAction>(player1);
    actionManager.addActionToTimeline(check_1);
    ASSERT_EQ(actionManager.getActiveBet(), 0);

    vector<PossibleAction> actual = actionManager.getAllowedActionTypes();
    vector<PossibleAction> expected = {{CHECK, 0}, {BET, 0}, {FOLD, 0}};
    ASSERT_EQ(actual, expected);
}

TEST_F(ActionTest, AllowedActionsToBigBlind) {
    auto postSmall_1 = make_shared<BlindAction>(player1, 1);
    auto postBig_2 = make_shared<BlindAction>(player2, 2);

    actionManager.addActionToTimeline(postSmall_1);
    actionManager.addActionToTimeline(postBig_2);

    vector<PossibleAction> actual = actionManager.getAllowedActionTypes();
    vector<PossibleAction> expected = {{CALL, 2}, {RAISE, 2}, {FOLD, 0}};
    ASSERT_EQ(actual, expected);
}

TEST_F(ActionTest, AllowedActionsToBet) {
    auto check_1 = make_shared<CheckAction>(player1);
    auto bet_2 = make_shared<BetAction>(player2, 10);

    actionManager.addActionToTimeline(check_1);
    ASSERT_EQ(actionManager.getActiveBet(), 0);
    actionManager.addActionToTimeline(bet_2);
    ASSERT_EQ(actionManager.getActiveBet(), 10);

    vector<PossibleAction> actual = actionManager.getAllowedActionTypes();
    vector<PossibleAction> expected = {{CALL, 10}, {RAISE, 10}, {FOLD, 0}};
    ASSERT_EQ(actual, expected);
}

TEST_F(ActionTest, AllowedActionsToRaise) {
    auto check_1 = make_shared<CheckAction>(player1);
    auto bet_2 = make_shared<BetAction>(player2, 10);
    auto fold_3 = make_shared<FoldAction>(player3);
    auto raise_4 = make_shared<RaiseAction>(player4, 40);

    actionManager.addActionToTimeline(check_1);
    ASSERT_EQ(actionManager.getActiveBet(), 0);
    actionManager.addActionToTimeline(bet_2);
    ASSERT_EQ(actionManager.getActiveBet(), 10);
    actionManager.addActionToTimeline(fold_3);
    ASSERT_EQ(actionManager.getActiveBet(), 10);
    actionManager.addActionToTimeline(raise_4);
    ASSERT_EQ(actionManager.getActiveBet(), 40);

    vector<PossibleAction> actual = actionManager.getAllowedActionTypes();
    vector<PossibleAction> expected = {{CALL, 40}, {RAISE, 40}, {FOLD, 0}};
    ASSERT_EQ(actual, expected);
}

TEST_F(ActionTest, AllowedActionsToCall) {
    auto check_1 = make_shared<CheckAction>(player1);
    auto bet_2 = make_shared<BetAction>(player2, 10);
    auto fold_3 = make_shared<FoldAction>(player3);
    auto call_4 = make_shared<CallAction>(player4, 10);

    actionManager.addActionToTimeline(check_1);
    ASSERT_EQ(actionManager.getActiveBet(), 0);
    actionManager.addActionToTimeline(bet_2);
    ASSERT_EQ(actionManager.getActiveBet(), 10);
    actionManager.addActionToTimeline(fold_3);
    ASSERT_EQ(actionManager.getActiveBet(), 10);
    actionManager.addActionToTimeline(call_4);
    ASSERT_EQ(actionManager.getActiveBet(), 10);

    vector<PossibleAction> actual = actionManager.getAllowedActionTypes();
    vector<PossibleAction> expected = {{CALL, 10}, {RAISE, 10}, {FOLD, 0}};
    ASSERT_EQ(actual, expected);
}

TEST_F(ActionTest, AllowedActionsToFold_1) {
    auto check_1 = make_shared<CheckAction>(player1);
    auto fold_2 = make_shared<FoldAction>(player2);

    actionManager.addActionToTimeline(check_1);
    ASSERT_EQ(actionManager.getActiveBet(), 0);
    actionManager.addActionToTimeline(fold_2);
    ASSERT_EQ(actionManager.getActiveBet(), 0);

    vector<PossibleAction> actual = actionManager.getAllowedActionTypes();
    vector<PossibleAction> expected = {{CHECK, 0} , {BET, 0}, {FOLD, 0}};
    ASSERT_EQ(actual, expected);
}

TEST_F(ActionTest, AllowedActionsToFold_2) {
    auto check_1 = make_shared<CheckAction>(player1);
    auto bet_2 = make_shared<BetAction>(player2, 10);
    auto fold_3 = make_shared<FoldAction>(player3);
    auto raise_4 = make_shared<RaiseAction>(player4, 40);
    auto fold_1 = make_shared<FoldAction>(player1);

    actionManager.addActionToTimeline(check_1);
    ASSERT_EQ(actionManager.getActiveBet(), 0);
    actionManager.addActionToTimeline(bet_2);
    ASSERT_EQ(actionManager.getActiveBet(), 10);
    actionManager.addActionToTimeline(fold_3);
    ASSERT_EQ(actionManager.getActiveBet(), 10);
    actionManager.addActionToTimeline(raise_4);
    ASSERT_EQ(actionManager.getActiveBet(), 40);
    actionManager.addActionToTimeline(fold_1);
    ASSERT_EQ(actionManager.getActiveBet(), 40);

    vector<PossibleAction> actual = actionManager.getAllowedActionTypes();
    vector<PossibleAction> expected = {{CALL, 40}, {RAISE, 40}, {FOLD, 0}};
    ASSERT_EQ(actual, expected);
}

TEST_F(ActionTest, BetAllIn) {
    auto check_1 = make_shared<CheckAction>(player1);
    auto bet_2 = make_shared<BetAction>(player2, 10);
    auto raise_3 = make_shared<RaiseAction>(player3, 500);

    actionManager.addActionToTimeline(check_1);
    actionManager.addActionToTimeline(bet_2);
    ASSERT_EQ(actionManager.getActiveBet(), 10);
    actionManager.addActionToTimeline(raise_3);
    ASSERT_EQ(actionManager.getActiveBet(), 500);

    vector<PossibleAction> actual = actionManager.getAllowedActionTypes();
    vector<PossibleAction> expected = {{CALL, 500}, {RAISE, 500}, {FOLD, 0}};
    ASSERT_EQ(actual, expected);
}

TEST_F(ActionTest, CallAllIn) {
    auto bet_1 = make_shared<BetAction>(player2, 1000);
    auto call_2 = make_shared<CallAction>(player3, 500);

    actionManager.addActionToTimeline(bet_1);
    ASSERT_EQ(actionManager.getActiveBet(), 1000);
    actionManager.addActionToTimeline(call_2);
    ASSERT_EQ(actionManager.getActiveBet(), 1000);

    vector<PossibleAction> actual = actionManager.getAllowedActionTypes();
    vector<PossibleAction> expected = {{CALL, 1000}, {RAISE, 1000}, {FOLD, 0}};
    ASSERT_EQ(actual, expected);
}

TEST_F(ActionTest, AllPlayersChecked) {
    auto check_1 = make_shared<CheckAction>(player1);
    auto check_2 = make_shared<CheckAction>(player2);
    auto check_3 = make_shared<CheckAction>(player3);
    auto check_4 = make_shared<CheckAction>(player4);

    actionManager.addActionToTimeline(check_1);
    actionManager.addActionToTimeline(check_2);
    actionManager.addActionToTimeline(check_3);
    actionManager.addActionToTimeline(check_4);

    ASSERT_EQ(actionManager.isActionsFinished(4), true);
}

TEST_F(ActionTest, BetCalledTrue) {
    auto blind_1 = make_shared<BlindAction>(player1, 2);
    auto blind_2 = make_shared<BlindAction>(player2, 3);
    auto raise_3 = make_shared<RaiseAction>(player3, 10);
    auto call_4 = make_shared<CallAction>(player4, 10);
    auto call_1 = make_shared<CallAction>(player1, 10);
    auto call_2 = make_shared<CallAction>(player2, 10);

    actionManager.addActionToTimeline(blind_1);
    actionManager.addActionToTimeline(blind_2);
    actionManager.addActionToTimeline(raise_3);
    actionManager.addActionToTimeline(call_4);
    actionManager.addActionToTimeline(call_1);
    actionManager.addActionToTimeline(call_2);

    ASSERT_EQ(actionManager.isActionsFinished(4), true);
}

TEST_F(ActionTest, BetCalledFalse) {
    auto blind_1 = make_shared<BlindAction>(player1, 2);
    auto blind_2 = make_shared<BlindAction>(player2, 3);
    auto call_3 = make_shared<CallAction>(player3, 3);
    auto raise_4 = make_shared<RaiseAction>(player4, 10);
    auto call_1 = make_shared<CallAction>(player1, 10);
    auto call_2 = make_shared<CallAction>(player2, 10);

    actionManager.addActionToTimeline(blind_1);
    actionManager.addActionToTimeline(blind_2);
    actionManager.addActionToTimeline(call_3);
    actionManager.addActionToTimeline(raise_4);
    actionManager.addActionToTimeline(call_1);
    actionManager.addActionToTimeline(call_2);

    ASSERT_EQ(actionManager.isActionsFinished(4), false);
}

TEST_F(ActionTest, BetFoldedTo) {
    auto bet_1 = make_shared<BetAction>(player1, 100);
    auto call_2 = make_shared<CallAction>(player2, 100);
    auto fold_3 = make_shared<FoldAction>(player3);
    auto raise_4 = make_shared<RaiseAction>(player4, 200);
    auto fold_1 = make_shared<FoldAction>(player1);
    auto fold_2 = make_shared<FoldAction>(player2);

    actionManager.addActionToTimeline(bet_1);
    actionManager.addActionToTimeline(call_2);
    actionManager.addActionToTimeline(fold_3);
    actionManager.addActionToTimeline(raise_4);
    actionManager.addActionToTimeline(fold_1);
    actionManager.addActionToTimeline(fold_2);

    ASSERT_EQ(actionManager.isActionsFinished(4), true);
}