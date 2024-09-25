#include <gtest/gtest.h>
#include "../include/GamePlayers.h"
#include "../include/Player.h"

TEST(GamePlayerTest, addPlayers) {
    GamePlayers gamePlayers;
    gamePlayers.addPlayerToGame("Jimmy", 100);
    gamePlayers.addPlayerToGame("Keven", 200);
    gamePlayers.addPlayerToGame("Hansuh", 300);
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 3);

    gamePlayers.addPlayerToGame("Kody", 400);
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 4);

    gamePlayers.displayPlayersInGame();
}

TEST(GamePlayerTest, removePlayers) {
    GamePlayers gamePlayers;
    gamePlayers.addPlayerToGame("Jimmy", 100);
    gamePlayers.addPlayerToGame("Keven", 200);
    gamePlayers.addPlayerToGame("Hansuh", 300);
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 3);

    gamePlayers.removePlayerFromGame("Hansuh");
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 2);

    gamePlayers.removePlayerFromGame("Jimmy");
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 1);

    gamePlayers.removePlayerFromGame("Unknown");
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 1);

    gamePlayers.removePlayerFromGame("Keven");
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 0);

    gamePlayers.displayPlayersInGame();
}

TEST(GamePlayerTest, addAndRemovePlayers) {
    GamePlayers gamePlayers;
    gamePlayers.addPlayerToGame("Jimmy", 100);
    gamePlayers.addPlayerToGame("Keven", 200);
    gamePlayers.addPlayerToGame("Hansuh", 300);
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 3);

    gamePlayers.removePlayerFromGame("Jimmy");
    gamePlayers.addPlayerToGame("Kody", 400);
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 3);

    gamePlayers.removePlayerFromGame("Keven");
    gamePlayers.addPlayerToGame("Sam", 400);
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 3);

    gamePlayers.removePlayerFromGame("Hansuh");
    gamePlayers.addPlayerToGame("Tevin", 400);
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 3);

    gamePlayers.displayPlayersInGame();
}