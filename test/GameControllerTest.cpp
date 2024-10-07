#include <gtest/gtest.h>
#include "../include/GameController.h"

TEST(GameControllerTest, GameLogicForSingleStreet) {
    GameController game(2, 3);

    game.addNewPlayer("Jimmy", 500);
    game.addNewPlayer("Victoria", 500);
    game.addNewPlayer("William", 500);
    game.addNewPlayer("Nora", 500);
    game.addNewPlayer("Maha", 500);

    game.startStreet(PRE_FLOP);
};

