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

    void addPlayersToHand(std::initializer_list<shared_ptr<Player>> players) {
        for (const auto& player : players) {
            turnManager.addPlayerInHand(player);
        }
    }
};

TEST_F(TurnTest, BlindsAndButtonSetCorrectly) {
    addPlayersToHand({p1, p2, p3, p4, p5});
    ASSERT_EQ(turnManager.getPlayerWithButton(), p5);

    turnManager.setSmallBlindToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);

    turnManager.setBigBlindToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p2);

    ASSERT_EQ(turnManager.getPlayerToAct(), p3);
    ASSERT_EQ(turnManager.getPlayerToAct(), p4);
    ASSERT_EQ(turnManager.getPlayerToAct(), turnManager.getPlayerWithButton());
}

TEST_F(TurnTest, NextToActAfterFolds) {
    addPlayersToHand({p1, p2, p3, p4});

    turnManager.setSmallBlindToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);
    turnManager.foldPlayerInHand(p1);

    ASSERT_EQ(turnManager.getPlayerToAct(), p2);
    turnManager.foldPlayerInHand(p2);

    ASSERT_EQ(turnManager.getPlayerToAct(), p3);
}

TEST_F(TurnTest, NextToActHeadsUp) {
    addPlayersToHand({p1, p2});

    turnManager.setSmallBlindToAct();
    ASSERT_EQ(turnManager.getPlayerWithButton(), p1);

    ASSERT_EQ(turnManager.getPlayerToAct(), p1);
    ASSERT_EQ(turnManager.getPlayerToAct(), p2);
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);
    ASSERT_EQ(turnManager.getPlayerToAct(), p2);
}

TEST_F(TurnTest, FoldPlayerToHeadsUp) {
    addPlayersToHand({p1, p2, p3});

    turnManager.setBigBlindToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p2);
    ASSERT_EQ(turnManager.getPlayerToAct(), p3);
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);

    ASSERT_EQ(turnManager.getPlayerToAct(), p2);
    turnManager.foldPlayerInHand(p2);

    ASSERT_EQ(turnManager.getPlayerToAct(), p3);
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);
    ASSERT_EQ(turnManager.getPlayerToAct(), p3);
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);
}

TEST_F(TurnTest, FoldPlayers) {
    addPlayersToHand({p1, p2, p3, p4, p5});

    turnManager.setEarlyPositionToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);

    ASSERT_EQ(turnManager.getPlayerToAct(), p2);
    turnManager.foldPlayerInHand(p2);

    ASSERT_EQ(turnManager.getPlayerToAct(), p3);

    ASSERT_EQ(turnManager.getPlayerToAct(), p4);
    turnManager.foldPlayerInHand(p4);

    ASSERT_EQ(turnManager.getPlayerToAct(), p5);
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);
    ASSERT_EQ(turnManager.getPlayerToAct(), p3);
    ASSERT_EQ(turnManager.getPlayerToAct(), p5);
}

TEST_F(TurnTest, ResetFoldedPlayers) {
    addPlayersToHand({p1, p2, p3, p4, p5});

    turnManager.setEarlyPositionToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);
    turnManager.foldPlayerInHand(p1);

    ASSERT_EQ(turnManager.getPlayerToAct(), p2);
    turnManager.foldPlayerInHand(p2);

    ASSERT_EQ(turnManager.getPlayerToAct(), p3);
    turnManager.foldPlayerInHand(p3);

    ASSERT_EQ(turnManager.getPlayerToAct(), p4);
    ASSERT_EQ(turnManager.getPlayerToAct(), p5);

    turnManager.moveFoldedPlayersToInHand();
    ASSERT_EQ(turnManager.getNumPlayersFolded(), 0);

    turnManager.setEarlyPositionToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);
}

TEST_F(TurnTest, TableRotationsHeadsUp) {
    addPlayersToHand({p1, p2});
    turnManager.rotatePositions();
    turnManager.setSmallBlindToAct();

    ASSERT_EQ(turnManager.getPlayerToAct(), turnManager.getPlayerWithButton());
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);
    ASSERT_EQ(turnManager.getPlayerToAct(), p2);
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);
}

TEST_F(TurnTest, TableRotations) {
    addPlayersToHand({p1, p2, p4, p6, p8});

    turnManager.rotatePositions();
    turnManager.setEarlyPositionToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p8);

    turnManager.rotatePositions();
    turnManager.setEarlyPositionToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p6);


    turnManager.rotatePositions();
    turnManager.setEarlyPositionToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p4);
}

TEST_F(TurnTest, RemoveSmall) {
    addPlayersToHand({p1, p2, p3, p4});
    turnManager.removePlayerFromHand(p1);

    turnManager.setSmallBlindToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p2);

    turnManager.setEarlyPositionToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p2);
}

TEST_F(TurnTest, RemoveBig) {
    addPlayersToHand({p1, p2, p3, p4});
    turnManager.removePlayerFromHand(p2);

    turnManager.setBigBlindToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p3);

    turnManager.setEarlyPositionToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);
}

TEST_F(TurnTest, RemoveBlinds) {
    addPlayersToHand({p1, p2, p3, p4, p5});
    turnManager.removePlayerFromHand(p1);
    turnManager.removePlayerFromHand(p2);

    turnManager.setEarlyPositionToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p3);

    turnManager.setBigBlindToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p4);

    turnManager.setSmallBlindToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p3);
}