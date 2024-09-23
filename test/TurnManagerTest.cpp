#include <gtest/gtest.h>
#include "../include/TurnManager.h"
#include "../include/Player.h"

class TurnTest : public ::testing::Test {
protected:
    TurnManager turnManager;
    shared_ptr<Player> player1;
    shared_ptr<Player> player2;
    shared_ptr<Player> player3;
    shared_ptr<Player> player4;
    shared_ptr<Player> player5;
    shared_ptr<Player> player6;
    shared_ptr<Player> player7;
    shared_ptr<Player> player8;
    shared_ptr<Player> player9;

    TurnTest() {
        player1 = make_shared<Player>("P1", Position::SMALL_BLIND, 500);
        player2 = make_shared<Player>("P2", Position::BIG_BLIND, 500);
        player3 = make_shared<Player>("P3", Position::UTG, 500);
        player4 = make_shared<Player>("P4", Position::UTG_1, 500);
        player5 = make_shared<Player>("P5", Position::MIDDLE, 500);
        player6 = make_shared<Player>("P6", Position::LOJACK, 500);
        player7 = make_shared<Player>("P7", Position::HIJACK, 500);
        player8 = make_shared<Player>("P8", Position::CUT_OFF, 500);
        player9 = make_shared<Player>("P9", Position::BUTTON, 500);

        turnManager.addPlayerInHand(player1);
        turnManager.addPlayerInHand(player2);
        turnManager.addPlayerInHand(player3);
        turnManager.addPlayerInHand(player4);
        turnManager.addPlayerInHand(player5);
        turnManager.addPlayerInHand(player6);
        turnManager.addPlayerInHand(player7);
        turnManager.addPlayerInHand(player8);
        turnManager.addPlayerInHand(player9);
    }

    void TearDown() override {
        turnManager.moveFoldedPlayersToInHand();
    }
};

TEST_F(TurnTest, SmallBlindFirstToAct) {
    ASSERT_EQ(turnManager.getEarlyPositionToAct(), player1);
}

TEST_F(TurnTest, FindNextToAct) {
    ASSERT_EQ(turnManager.getBigBlindToAct(), player2);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), player3);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), player4);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), player5);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), player6);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), player7);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), player8);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), player9);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), player1);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), player2);
    turnManager.displayPlayerToAct();
}

TEST_F(TurnTest, RotatePositions) {
    ASSERT_EQ(turnManager.getBigBlindToAct(), player2);
    turnManager.rotatePositions();

    ASSERT_EQ(turnManager.getBigBlindToAct(), player1);

    turnManager.rotatePositions();
    ASSERT_EQ(turnManager.getBigBlindToAct(), player9); 

    turnManager.rotatePositions();
    ASSERT_EQ(turnManager.getBigBlindToAct(), player8);

    turnManager.rotatePositions();
    ASSERT_EQ(turnManager.getBigBlindToAct(), player7);

    turnManager.rotatePositions();
    ASSERT_EQ(turnManager.getBigBlindToAct(), player6);

    turnManager.rotatePositions();
    ASSERT_EQ(turnManager.getBigBlindToAct(), player5);

    turnManager.rotatePositions();
    ASSERT_EQ(turnManager.getBigBlindToAct(), player4);

    turnManager.rotatePositions();
    ASSERT_EQ(turnManager.getBigBlindToAct(), player3);

    turnManager.rotatePositions();
    ASSERT_EQ(turnManager.getBigBlindToAct(), player2);
}

TEST_F(TurnTest, FoldPlayer) {
    ASSERT_EQ(turnManager.getBigBlindToAct(), player2);
    ASSERT_EQ(turnManager.getNextToAct(), player3);
    ASSERT_EQ(turnManager.getNextToAct(), player4);

    turnManager.foldPlayerInHand(player4);
    ASSERT_EQ(turnManager.getNumPlayersInHand(), 8);
    ASSERT_EQ(turnManager.getNumPlayersFolded(), 1);

    ASSERT_EQ(turnManager.getNextToAct(), player5);

    turnManager.foldPlayerInHand(player5);
    ASSERT_EQ(turnManager.getNumPlayersInHand(), 7);
    ASSERT_EQ(turnManager.getNumPlayersFolded(), 2);

    ASSERT_EQ(turnManager.getNextToAct(), player6);
    ASSERT_EQ(turnManager.getNextToAct(), player7);
    ASSERT_EQ(turnManager.getNextToAct(), player8);
    ASSERT_EQ(turnManager.getNextToAct(), player9);
    ASSERT_EQ(turnManager.getNextToAct(), player1);
    ASSERT_EQ(turnManager.getNextToAct(), player2);
    ASSERT_EQ(turnManager.getNextToAct(), player3);

    // Player 4 and 5 are folded, so we skip to player 6
    ASSERT_EQ(turnManager.getNextToAct(), player6);
    ASSERT_EQ(turnManager.getNextToAct(), player7);
    ASSERT_EQ(turnManager.getNextToAct(), player8);
    ASSERT_EQ(turnManager.getNextToAct(), player9);
}

TEST_F(TurnTest, EarlyPosition) {
    turnManager.foldPlayerInHand(player1);
    turnManager.foldPlayerInHand(player2);
    turnManager.foldPlayerInHand(player3);
    ASSERT_EQ(turnManager.getEarlyPositionToAct(), player4);
}

TEST_F(TurnTest, ResetFoldedPlayers) {
    turnManager.foldPlayerInHand(player1);
    turnManager.foldPlayerInHand(player2);
    turnManager.foldPlayerInHand(player3);
    ASSERT_EQ(turnManager.getNumPlayersFolded(), 3);

    turnManager.moveFoldedPlayersToInHand();
    ASSERT_EQ(turnManager.getNumPlayersFolded(), 0);

    ASSERT_EQ(turnManager.getEarlyPositionToAct(), player1);
    ASSERT_EQ(turnManager.getBigBlindToAct(), player2);
}