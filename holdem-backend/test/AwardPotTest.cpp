#include <gtest/gtest.h>
#include "../include/Player.h"
#include "../include/PotManager.h"
TEST(AwardPotTest, MultiPotCalculation) {
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
    potManager.displayPots();

    vector<shared_ptr<Player>> sortedPlayers = {playerC, playerB, playerD, playerE, playerA};
    potManager.awardPots(sortedPlayers);

    EXPECT_EQ(playerA->getChips(), 100);
    EXPECT_EQ(playerB->getChips(), 300);
    EXPECT_EQ(playerC->getChips(), 2700);
    EXPECT_EQ(playerD->getChips(), 0);
    EXPECT_EQ(playerE->getChips(), 1700);
}