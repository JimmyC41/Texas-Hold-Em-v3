#include <gtest/gtest.h>
#include "../include/GameController.h"

TEST(GameControllerTest, GameLogicForSingleRound) {
    GameController game(2, 3);
    game.main();
};
