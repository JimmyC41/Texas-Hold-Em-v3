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

TEST(GamePlayerTest, removeBigBlind) {
    GamePlayers gamePlayers;
    gamePlayers.addPlayerToGame("Jimmy", 100);
    gamePlayers.addPlayerToGame("Keven", 200);
    gamePlayers.addPlayerToGame("Hansuh", 300);
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 3);

    gamePlayers.removePlayerFromGame("Keven");
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 2);

    gamePlayers.displayPlayersInGame();
}

TEST(GamePlayerTest, addAndRemovePlayers) {
    GamePlayers gamePlayers;

    gamePlayers.addPlayerToGame("Jimmy", 100);
    gamePlayers.addPlayerToGame("Keven", 100);
    gamePlayers.addPlayerToGame("Hansuh", 100);
    gamePlayers.addPlayerToGame("James", 100);
    gamePlayers.addPlayerToGame("Edison", 100);
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 5);

    gamePlayers.displayPlayersInGame();

    gamePlayers.removePlayerFromGame("Jimmy");
    gamePlayers.addPlayerToGame("Kody", 100);
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 5);

    gamePlayers.displayPlayersInGame();

    gamePlayers.removePlayerFromGame("Keven");
    gamePlayers.addPlayerToGame("Sam", 400);
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 5);

    gamePlayers.displayPlayersInGame();

    gamePlayers.removePlayerFromGame("Hansuh");
    gamePlayers.addPlayerToGame("Tevin", 400);
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 5);

    gamePlayers.displayPlayersInGame();

    gamePlayers.removePlayerFromGame("James");
    gamePlayers.addPlayerToGame("Olivia", 400);
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 5);

    gamePlayers.displayPlayersInGame();

    gamePlayers.removePlayerFromGame("Edison");
    gamePlayers.addPlayerToGame("Victoria", 400);
    ASSERT_EQ(gamePlayers.getNumPlayersInGame(), 5);

    gamePlayers.displayPlayersInGame();
}