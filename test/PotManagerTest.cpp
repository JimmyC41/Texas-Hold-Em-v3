#include <gtest/gtest.h>
#include "../include/Player.h"
#include "../include/PotManager.h"

TEST(PotManagerTest, PotCalculation1) {
    PotManager potManager;

    auto playerA = make_shared<Player>("Player A", Position::SMALL_BLIND, 1000);
    auto playerB = make_shared<Player>("Player B", Position::BIG_BLIND, 800);
    auto playerC = make_shared<Player>("Player C", Position::UTG, 600);
    auto playerD = make_shared<Player>("Player D", Position::UTG_1, 400);
    auto playerE = make_shared<Player>("Player E", Position::MIDDLE, 2000);

    potManager.addPlayerBet(playerA, 200, false);  // A bet 200
    potManager.addPlayerBet(playerB, 500, false);  // B raise 500
    potManager.addPlayerBet(playerC, 500, false);  // C call 500
    potManager.addPlayerBet(playerD, 400, true);  // D all-in call 400
    potManager.addPlayerBet(playerE, 900, false);  // E raise 900
    potManager.addPlayerBet(playerA, 900, false);  // A call 900
    potManager.foldPlayerBet(playerB);      // B fold (dead 500)
    potManager.addPlayerBet(playerC, 600, true);  // C all-in call 600

    potManager.displayPlayerBets();
    potManager.calculatePots();
    ASSERT_EQ(potManager.getNumPots(), 3);
    ASSERT_EQ(potManager.getPot(0).getChips(), 2000);
    ASSERT_EQ(potManager.getPot(1).getChips(), 700);
    ASSERT_EQ(potManager.getPot(2).getChips(), 600);
    potManager.displayPots();
}

TEST(PotManagerTest, PotCalculation2) {
    PotManager potManager;

    auto playerA = make_shared<Player>("Player A", Position::SMALL_BLIND, 1000);
    auto playerB = make_shared<Player>("Player B", Position::BIG_BLIND, 800);
    auto playerC = make_shared<Player>("Player C", Position::UTG, 600);
    auto playerD = make_shared<Player>("Player D", Position::UTG_1, 400);
    auto playerE = make_shared<Player>("Player E", Position::MIDDLE, 2000);

    potManager.addPlayerBet(playerA, 200, false);  // A bet 200
    potManager.addPlayerBet(playerB, 500, false);  // B raise 500
    potManager.addPlayerBet(playerC, 500, false);  // C call 500
    potManager.addPlayerBet(playerD, 400, true);  // D all-in call 400
    potManager.addPlayerBet(playerE, 900, false);  // E raise 900
    potManager.addPlayerBet(playerA, 900, false);  // A call 900
    potManager.foldPlayerBet(playerB);      // B fold (dead 500)
    potManager.addPlayerBet(playerC, 600, true);  // C all-in call 600

    potManager.displayPlayerBets();
    potManager.calculatePots();
    ASSERT_EQ(potManager.getNumPots(), 3);
    ASSERT_EQ(potManager.getPot(0).getChips(), 2000);
    ASSERT_EQ(potManager.getPot(1).getChips(), 700);
    ASSERT_EQ(potManager.getPot(2).getChips(), 600);
    potManager.displayPots();
}