#include <gtest/gtest.h>
#include "../include/TurnManager.h"
#include "../include/Player.h"

class TurnTest : public ::testing::Test {
protected:
    TurnManager turnManager;
    shared_ptr<Player> p1;
    shared_ptr<Player> p2;
    shared_ptr<Player> p3;
    shared_ptr<Player> p4;
    shared_ptr<Player> p5;
    shared_ptr<Player> p6;
    shared_ptr<Player> p7;
    shared_ptr<Player> p8;
    shared_ptr<Player> p9;

    TurnTest() {
        p1 = make_shared<Player>("P1", Position::SMALL_BLIND, 500);
        p2 = make_shared<Player>("P2", Position::BIG_BLIND, 500);
        p3 = make_shared<Player>("P3", Position::UTG, 500);
        p4 = make_shared<Player>("P4", Position::UTG_1, 500);
        p5 = make_shared<Player>("P5", Position::MIDDLE, 500);
        p6 = make_shared<Player>("P6", Position::LOJACK, 500);
        p7 = make_shared<Player>("P7", Position::HIJACK, 500);
        p8 = make_shared<Player>("P8", Position::CUT_OFF, 500);
        p9 = make_shared<Player>("P9", Position::DEALER, 500);
    }

    void TearDown() override {
        turnManager.moveFoldedPlayersToInHand();
    }
};

TEST_F(TurnTest, SmallBlindFirstToAct) {
    turnManager.addPlayerInHand(p1);
    turnManager.addPlayerInHand(p2);
    turnManager.addPlayerInHand(p3);
    ASSERT_EQ(turnManager.getEarlyPositionToAct(), p1);
    ASSERT_EQ(turnManager.getSmallBlindToAct(), p1);
}

TEST_F(TurnTest, FindNextToAct) {
    turnManager.addPlayerInHand(p1);
    turnManager.addPlayerInHand(p2);
    turnManager.addPlayerInHand(p4);
    turnManager.addPlayerInHand(p6);
    turnManager.addPlayerInHand(p8);
    ASSERT_EQ(turnManager.getPlayerWithButton(), p8);

    ASSERT_EQ(turnManager.getBigBlindToAct(), p2);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p4);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p6);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p8); 
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p1);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p2);
    turnManager.displayPlayerToAct();
}

TEST_F(TurnTest, NextToActHeadsUp) {
    turnManager.addPlayerInHand(p1);
    turnManager.addPlayerInHand(p2);
    ASSERT_EQ(turnManager.getPlayerWithButton(), p1);

    ASSERT_EQ(turnManager.getSmallBlindToAct(), p1);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p2);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p1);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p2);
    turnManager.displayPlayerToAct();
}

TEST_F(TurnTest, NextToAct) {
    turnManager.addPlayerInHand(p1);
    turnManager.addPlayerInHand(p2);
    turnManager.addPlayerInHand(p4);
    turnManager.addPlayerInHand(p5);
    turnManager.addPlayerInHand(p7);
    turnManager.addPlayerInHand(p9);
    ASSERT_EQ(turnManager.getPlayerWithButton(), p9);

    ASSERT_EQ(turnManager.getBigBlindToAct(), p2);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p4);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p5);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p7);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p9);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p1);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p2);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p4);
    turnManager.displayPlayerToAct();

    ASSERT_EQ(turnManager.getNextToAct(), p5);
    turnManager.displayPlayerToAct();
}

TEST_F(TurnTest, FoldPlayerToHeadsUp) {
    turnManager.addPlayerInHand(p1);
    turnManager.addPlayerInHand(p2);
    turnManager.addPlayerInHand(p3);
    turnManager.displayPlayersInHand();
    ASSERT_EQ(turnManager.getPlayerWithButton(), p3);

    ASSERT_EQ(turnManager.getBigBlindToAct(), p2);
    ASSERT_EQ(turnManager.getNextToAct(), p3);
    ASSERT_EQ(turnManager.getNextToAct(), p1);
    ASSERT_EQ(turnManager.getNextToAct(), p2);

    turnManager.foldPlayerInHand(p3);
    turnManager.displayPlayersInHand();
    ASSERT_EQ(turnManager.getNumPlayersInHand(), 2);
    ASSERT_EQ(turnManager.getNumPlayersFolded(), 1);
    
    ASSERT_EQ(turnManager.getNextToAct(), p1);
    ASSERT_EQ(turnManager.getNextToAct(), p2);
}

TEST_F(TurnTest, FoldPlayers) {
    turnManager.addPlayerInHand(p1);
    turnManager.addPlayerInHand(p2);
    turnManager.addPlayerInHand(p5);
    turnManager.addPlayerInHand(p6);
    turnManager.addPlayerInHand(p9);
    turnManager.displayPlayersInHand();
    ASSERT_EQ(turnManager.getPlayerWithButton(), p9);

    ASSERT_EQ(turnManager.getBigBlindToAct(), p2);
    ASSERT_EQ(turnManager.getNextToAct(), p5);
    ASSERT_EQ(turnManager.getNextToAct(), p6);
    ASSERT_EQ(turnManager.getNextToAct(), p9);
    ASSERT_EQ(turnManager.getNextToAct(), p1);

    turnManager.foldPlayerInHand(p2);
    turnManager.displayPlayersInHand();
    ASSERT_EQ(turnManager.getNumPlayersInHand(), 4);
    ASSERT_EQ(turnManager.getNumPlayersFolded(), 1);
    
    ASSERT_EQ(turnManager.getNextToAct(), p5);
    ASSERT_EQ(turnManager.getNextToAct(), p6);

    turnManager.foldPlayerInHand(p9);
    turnManager.displayPlayersInHand();
    ASSERT_EQ(turnManager.getNumPlayersInHand(), 3);
    ASSERT_EQ(turnManager.getNumPlayersFolded(), 2);

    ASSERT_EQ(turnManager.getNextToAct(), p1);
    ASSERT_EQ(turnManager.getNextToAct(), p5);
    ASSERT_EQ(turnManager.getNextToAct(), p6);
}

TEST_F(TurnTest, EarlyPosition) {
    turnManager.addPlayerInHand(p1);
    turnManager.addPlayerInHand(p2);
    turnManager.addPlayerInHand(p3);
    turnManager.addPlayerInHand(p4);
    turnManager.addPlayerInHand(p5);
    turnManager.displayPlayersInHand();
    ASSERT_EQ(turnManager.getPlayerWithButton(), p5);

    turnManager.foldPlayerInHand(p1);
    turnManager.foldPlayerInHand(p2);
    turnManager.foldPlayerInHand(p3);
    turnManager.displayPlayersInHand();
    ASSERT_EQ(turnManager.getEarlyPositionToAct(), p4);
}

TEST_F(TurnTest, ResetFoldedPlayers) {
    turnManager.addPlayerInHand(p1);
    turnManager.addPlayerInHand(p2);
    turnManager.addPlayerInHand(p3);
    turnManager.addPlayerInHand(p4);
    turnManager.addPlayerInHand(p5);
    turnManager.displayPlayersInHand();
    ASSERT_EQ(turnManager.getPlayerWithButton(), p5);

    turnManager.foldPlayerInHand(p1);
    turnManager.foldPlayerInHand(p2);
    turnManager.foldPlayerInHand(p3);
    turnManager.displayPlayersInHand();
    ASSERT_EQ(turnManager.getNumPlayersFolded(), 3);

    turnManager.moveFoldedPlayersToInHand();
    turnManager.displayPlayersInHand();
    ASSERT_EQ(turnManager.getNumPlayersFolded(), 0);

    ASSERT_EQ(turnManager.getEarlyPositionToAct(), p1);
    ASSERT_EQ(turnManager.getBigBlindToAct(), p2);
}

TEST_F(TurnTest, TableRotationsHeadsUp) {
    turnManager.addPlayerInHand(p1);
    turnManager.addPlayerInHand(p2);
    turnManager.displayPlayersInHand();
    ASSERT_EQ(turnManager.getPlayerWithButton(), p1);
    ASSERT_EQ(turnManager.getSmallBlindToAct(), p1);
    ASSERT_EQ(turnManager.getNextToAct(), p2);
   
   turnManager.rotatePositions();
   turnManager.displayPlayersInHand();
   ASSERT_EQ(turnManager.getPlayerWithButton(), p2);
   ASSERT_EQ(turnManager.getSmallBlindToAct(), p2);
   ASSERT_EQ(turnManager.getNextToAct(), p1);
}

TEST_F(TurnTest, TableRotations) {
    turnManager.addPlayerInHand(p1);
    turnManager.addPlayerInHand(p2);
    turnManager.addPlayerInHand(p4);
    turnManager.addPlayerInHand(p6);
    turnManager.addPlayerInHand(p8);
    turnManager.displayPlayersInHand();
    ASSERT_EQ(turnManager.getPlayerWithButton(), p8);
    ASSERT_EQ(turnManager.getEarlyPositionToAct(), p1);
    ASSERT_EQ(turnManager.getSmallBlindToAct(), p1);
    ASSERT_EQ(turnManager.getBigBlindToAct(), p2);

    turnManager.rotatePositions();
    turnManager.displayPlayersInHand();
    ASSERT_EQ(turnManager.getPlayerWithButton(), p6);
    ASSERT_EQ(turnManager.getEarlyPositionToAct(), p8);
    ASSERT_EQ(turnManager.getSmallBlindToAct(), p8);
    ASSERT_EQ(turnManager.getBigBlindToAct(), p1);

    turnManager.rotatePositions();
    turnManager.displayPlayersInHand();
    ASSERT_EQ(turnManager.getPlayerWithButton(), p4);
    ASSERT_EQ(turnManager.getEarlyPositionToAct(), p6);
    ASSERT_EQ(turnManager.getSmallBlindToAct(), p6);
    ASSERT_EQ(turnManager.getBigBlindToAct(), p8);
}