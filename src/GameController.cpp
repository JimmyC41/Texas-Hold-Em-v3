#include "../include/GameController.h"

GameController::GameController(size_t smallBlind, size_t bigBlind) :
    smallBlind(smallBlind),
    bigBlind(bigBlind),
    deck(),
    board(),
    dealer(Dealer::createDealer(deck, board)),
    gamePlayers(),
    actionManager(),
    turnManager(),
    clientManager(bigBlind),
    potManager() {}

// GAME STATE INFORMATION METHODS
int GameController::getNumPlayersInGame() const {
    return gamePlayers.getNumPlayersInGame();
}

void GameController::displayPlayersInGame() const {
    gamePlayers.displayPlayersInGame();
}

// PLAYER SPECIFIC METHODS

void GameController::addNewPlayer(const string& name, size_t chips) {
    shared_ptr<Player> newPlayer = gamePlayers.addPlayerToGame(name, chips);
    if (newPlayer != nullptr) turnManager.addPlayerInHand(newPlayer);
}

void GameController::removePlayer(const string& name) {
    shared_ptr<Player> oldPlayer = gamePlayers.removePlayerFromGame(name);
    if (oldPlayer != nullptr) turnManager.removePlayerFromHand(oldPlayer);
}

// DEALER SPECIFIC METHODS

void GameController::dealPlayers() {
    cout << "--------Dealing cards to players!-------" << endl;
    for (int i = 0; i < 2; i++) {
        for (const auto& player : turnManager.getPlayersInHand()) {
            dealer.dealPlayer(player);
        }
    }
    cout << "----------------------------------------\n" << endl;
}

void GameController::dealBoard(int numCards) {
    cout << "----------------------------------------" << endl;
    cout << "Dealing " << numCards << " community card(s) to the board!" << endl;
    dealer.dealBoard(numCards, 1);
    cout << "----------------------------------------\n" << endl;
}

// ROUND SPECIFIC METHODS

