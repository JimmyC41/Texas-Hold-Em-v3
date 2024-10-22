#include <gtest/gtest.h>
#include "../include/Player.h"
#include "../include/ActionManager.h"
#include "../include/Action.h"
#include "../include/Actions/BetAction.h"
#include "../include/Actions/AllInBetAction.h"
#include "../include/Actions/AllInCallAction.h"
#include "../include/Actions/BlindAction.h"
#include "../include/Actions/CallAction.h"
#include "../include/Actions/CheckAction.h"
#include "../include/Actions/FoldAction.h"
#include "../include/Actions/RaiseAction.h"

class AllInTest : public ::testing::Test {
protected:
    ActionManager actionManager;
    shared_ptr<Player> player1;
    shared_ptr<Player> player2;
    shared_ptr<Player> player3;

    AllInTest() {
        player1 = make_shared<Player>("P1", Position::SMALL_BLIND, 1000);
        player2 = make_shared<Player>("P2", Position::BIG_BLIND, 2000);
        player3 = make_shared<Player>("P3", Position::UTG, 3000);
    }

    void TearDown() override {
        actionManager.clearActionTimeline();
    }
};

TEST_F(AllInTest, AB) {
    auto a_bet_1 = make_shared<AllInBetAction>(player1, 1000);
    auto call_2 = make_shared<CallAction>(player2, 1000);
    auto call_3 = make_shared<CallAction>(player3, 1000);

    actionManager.addActionToTimeline(a_bet_1);
    actionManager.addActionToTimeline(call_2);
    actionManager.addActionToTimeline(call_3);

    ASSERT_EQ(actionManager.getActiveBet(), 1000);
    ASSERT_EQ(actionManager.isActionsFinished(3), true);
}

TEST_F(AllInTest, AR) {
    auto bet_1 = make_shared<BetAction>(player1, 500);
    auto a_raise_2 = make_shared<AllInBetAction>(player2, 2000);
    auto call_3 = make_shared<CallAction>(player3, 2000);
    auto fold_1 = make_shared<FoldAction>(player1);

    actionManager.addActionToTimeline(bet_1);
    actionManager.addActionToTimeline(a_raise_2);
    actionManager.addActionToTimeline(call_3);
    actionManager.addActionToTimeline(fold_1);

    ASSERT_EQ(actionManager.getActiveBet(), 2000);
    ASSERT_EQ(actionManager.isActionsFinished(3), true);
}

TEST_F(AllInTest, AC) {
    auto check_1 = make_shared<CheckAction>(player1);
    auto bet_2 = make_shared<BetAction>(player2, 1500);
    auto call_3 = make_shared<CallAction>(player3, 1500);
    auto a_call_1 = make_shared<AllInCallAction>(player1, 1000);

    actionManager.addActionToTimeline(check_1);
    actionManager.addActionToTimeline(bet_2);
    actionManager.addActionToTimeline(call_3);
    actionManager.addActionToTimeline(a_call_1);

    ASSERT_EQ(actionManager.getActiveBet(), 1500);
    ASSERT_EQ(actionManager.isActionsFinished(3), true);
}

TEST_F(AllInTest, AB_AR) {
    auto a_bet_1 = make_shared<AllInBetAction>(player1, 1000);
    auto a_raise_2 = make_shared<AllInBetAction>(player2, 2000);
    auto call_3 = make_shared<CallAction>(player3, 2000);

    actionManager.addActionToTimeline(a_bet_1);
    actionManager.addActionToTimeline(a_raise_2);
    actionManager.addActionToTimeline(call_3);

    ASSERT_EQ(actionManager.getActiveBet(), 2000);
    ASSERT_EQ(actionManager.isActionsFinished(3), true);
}

TEST_F(AllInTest, AB_AC) {
    auto check_1 = make_shared<CheckAction>(player1);
    auto a_bet_2 = make_shared<AllInBetAction>(player2, 2000);
    auto fold_3 = make_shared<FoldAction>(player3);
    auto a_call_1 = make_shared<AllInCallAction>(player1, 1000);

    actionManager.addActionToTimeline(check_1);
    actionManager.addActionToTimeline(a_bet_2);
    actionManager.addActionToTimeline(fold_3);
    actionManager.addActionToTimeline(a_call_1);

    ASSERT_EQ(actionManager.getActiveBet(), 2000);
    ASSERT_EQ(actionManager.isActionsFinished(3), true);
}

TEST_F(AllInTest, AR_AC) {
    auto bet_1 = make_shared<BetAction>(player1, 500);
    auto a_bet_2 = make_shared<AllInBetAction>(player2, 2000);
    auto call_3 = make_shared<CallAction>(player3, 2000);
    auto a_call_1 = make_shared<AllInCallAction>(player1, 1000);

    actionManager.addActionToTimeline(bet_1);
    actionManager.addActionToTimeline(a_bet_2);
    actionManager.addActionToTimeline(call_3);
    actionManager.addActionToTimeline(a_call_1);

    ASSERT_EQ(actionManager.getActiveBet(), 2000);
    ASSERT_EQ(actionManager.isActionsFinished(3), true);
}

TEST_F(AllInTest, AC_AR) {
    auto check_1 = make_shared<CheckAction>(player1);
    auto check_2 = make_shared<CheckAction>(player1);
    auto bet_3 = make_shared<BetAction>(player3, 1500);
    auto a_call_1 = make_shared<AllInCallAction>(player1, 1000);
    auto a_raise_2 = make_shared<AllInBetAction>(player2, 2000);
    auto call_3 = make_shared<CallAction>(player3, 2000);

    actionManager.addActionToTimeline(check_1);
    actionManager.addActionToTimeline(check_2);
    actionManager.addActionToTimeline(bet_3);
    actionManager.addActionToTimeline(a_call_1);
    actionManager.addActionToTimeline(a_raise_2);
    actionManager.addActionToTimeline(call_3);

    ASSERT_EQ(actionManager.getActiveBet(), 2000);
    ASSERT_EQ(actionManager.isActionsFinished(3), true);
}

TEST_F(AllInTest, AB_AR_AC) {
    shared_ptr<Player> player4 = make_shared<Player>("P4", Position::UTG_1, 500);

    auto a_check_1 = make_shared<CheckAction>(player1);
    auto a_raise_2 = make_shared<AllInBetAction>(player2, 2000);
    auto a_raise_3 = make_shared<AllInBetAction>(player3, 3000);
    auto a_call_4 = make_shared<AllInCallAction>(player4, 500);
    auto a_fold_1 = make_shared<FoldAction>(player1);

    actionManager.addActionToTimeline(a_check_1);
    actionManager.addActionToTimeline(a_raise_2);
    actionManager.addActionToTimeline(a_raise_3);
    actionManager.addActionToTimeline(a_call_4);
    actionManager.addActionToTimeline(a_fold_1);

    ASSERT_EQ(actionManager.getActiveBet(), 3000);
    ASSERT_EQ(actionManager.isActionsFinished(4), true);
}

TEST_F(AllInTest, AllActions) {
    shared_ptr<Player> player4 = make_shared<Player>("P4", Position::UTG, 4000);
    shared_ptr<Player> player5 = make_shared<Player>("P5", Position::UTG_1, 500);
    shared_ptr<Player> player6 = make_shared<Player>("P6", Position::LOJACK, 10000);
    shared_ptr<Player> player7 = make_shared<Player>("P7", Position::HIJACK, 2500);
    shared_ptr<Player> player8 = make_shared<Player>("P8", Position::CUT_OFF, 100);
    shared_ptr<Player> player9 = make_shared<Player>("P9", Position::DEALER, 5000);

    auto check_1 = make_shared<CheckAction>(player1);
    auto raise_2 = make_shared<BetAction>(player2, 100);
    auto raise_3 = make_shared<RaiseAction>(player3, 500);
    auto call_4 = make_shared<CallAction>(player4, 500);
    auto a_call_5 = make_shared<AllInCallAction>(player5, 500);
    auto raise_6 = make_shared<RaiseAction>(player6, 2000);
    auto a_raise_7 = make_shared<AllInBetAction>(player7, 2500);
    auto a_call_8 = make_shared<AllInCallAction>(player8, 100);
    auto a_raise_9 = make_shared<AllInBetAction>(player9, 5000);
    auto fold_1 = make_shared<FoldAction>(player1);
    auto fold_2 = make_shared<FoldAction>(player2);
    auto a_call_3 = make_shared<AllInCallAction>(player3, 3000);
    auto fold_4 = make_shared<FoldAction>(player4);
    auto call_6 = make_shared<CallAction>(player6, 5000);

    actionManager.addActionToTimeline(check_1);
    actionManager.addActionToTimeline(raise_2);
    actionManager.addActionToTimeline(raise_3);
    actionManager.addActionToTimeline(call_4);
    actionManager.addActionToTimeline(a_call_5);
    actionManager.addActionToTimeline(raise_6);
    actionManager.addActionToTimeline(a_raise_7);
    actionManager.addActionToTimeline(a_call_8);
    actionManager.addActionToTimeline(a_raise_9);
    actionManager.addActionToTimeline(fold_1);
    actionManager.addActionToTimeline(fold_2);
    actionManager.addActionToTimeline(a_call_3);
    actionManager.addActionToTimeline(fold_4);
    actionManager.addActionToTimeline(call_6);

    ASSERT_EQ(actionManager.getActiveBet(), 5000);
    ASSERT_EQ(actionManager.isActionsFinished(9), true);
}

