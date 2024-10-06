#include <gtest/gtest.h>
#include "../include/GameController.h"

TEST(GameControllerTest, gamePlayers) {
    GameController game(2, 3);

    game.addNewPlayer("Jimmy", 500);
    game.addNewPlayer("Victoria", 500);
    game.addNewPlayer("William", 500);
    game.addNewPlayer("Nora", 500);
    game.addNewPlayer("Maha", 500);
    game.displayPlayersInGame();
    ASSERT_EQ(game.getNumPlayersInGame(), 5);

    game.removePlayer("Jimmy");
    game.removePlayer("Victoria");
    game.displayPlayersInGame();
    ASSERT_EQ(game.getNumPlayersInGame(), 3);

    game.addNewPlayer("Jimothy", 500);
    game.displayPlayersInGame();
    game.addNewPlayer("Sophie", 500);
    game.displayPlayersInGame();
    ASSERT_EQ(game.getNumPlayersInGame(), 5);

    game.removePlayer("William");
    game.removePlayer("Nora");
    game.removePlayer("Maha");
    game.displayPlayersInGame();
    ASSERT_EQ(game.getNumPlayersInGame(), 2);
};

TEST(GameControllerTest, dealingCards) {
    GameController game(2, 3);

    game.addNewPlayer("Jimmy", 500);
    game.addNewPlayer("Victoria", 500);
    game.addNewPlayer("William", 500);
    game.addNewPlayer("Nora", 500);
    game.addNewPlayer("Maha", 500);

    game.dealPlayers();
    game.dealBoard(3);
    game.dealBoard(1);
    game.dealBoard(1);
};