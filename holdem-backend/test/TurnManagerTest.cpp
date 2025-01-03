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
        p1 = make_shared<Player>("P1", Position::SMALL_BLIND, 10);
        p2 = make_shared<Player>("P2", Position::BIG_BLIND, 20);
        p3 = make_shared<Player>("P3", Position::UTG, 30);
        p4 = make_shared<Player>("P4", Position::UTG_1, 40);
        p5 = make_shared<Player>("P5", Position::MIDDLE, 50);
        p6 = make_shared<Player>("P6", Position::LOJACK, 60);
        p7 = make_shared<Player>("P7", Position::HIJACK, 70);
        p8 = make_shared<Player>("P8", Position::CUT_OFF, 80);
        p9 = make_shared<Player>("P9", Position::DEALER, 90);
    }

    void TearDown() override {
        turnManager.moveAllPlayersToInHand();
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
    turnManager.addPlayerNotInHand(p1);

    ASSERT_EQ(turnManager.getPlayerToAct(), p2);
    turnManager.addPlayerNotInHand(p2);

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
    turnManager.addPlayerNotInHand(p2);

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
    turnManager.addPlayerNotInHand(p2);

    ASSERT_EQ(turnManager.getPlayerToAct(), p3);

    ASSERT_EQ(turnManager.getPlayerToAct(), p4);
    turnManager.addPlayerNotInHand(p4);

    ASSERT_EQ(turnManager.getPlayerToAct(), p5);
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);
    ASSERT_EQ(turnManager.getPlayerToAct(), p3);
    ASSERT_EQ(turnManager.getPlayerToAct(), p5);
}

TEST_F(TurnTest, ResetFoldedPlayers) {
    addPlayersToHand({p1, p2, p3, p4, p5});

    turnManager.setEarlyPositionToAct();
    ASSERT_EQ(turnManager.getPlayerToAct(), p1);
    turnManager.addPlayerNotInHand(p1);

    ASSERT_EQ(turnManager.getPlayerToAct(), p2);
    turnManager.addPlayerNotInHand(p2);

    ASSERT_EQ(turnManager.getPlayerToAct(), p3);
    turnManager.addPlayerNotInHand(p3);

    ASSERT_EQ(turnManager.getPlayerToAct(), p4);
    ASSERT_EQ(turnManager.getPlayerToAct(), p5);

    turnManager.moveAllPlayersToInHand();
    ASSERT_EQ(turnManager.getNumPlayersNotInHand(), 0);

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

TEST_F(TurnTest, BigStackTest) {
    addPlayersToHand({p1, p2, p3, p4});
    ASSERT_EQ(turnManager.getBigStackChipCount(), 40);

    addPlayersToHand({p5});
    ASSERT_EQ(turnManager.getBigStackChipCount(), 50);

    addPlayersToHand({p6, p7, p8});
    ASSERT_EQ(turnManager.getBigStackChipCount(), 80);
}