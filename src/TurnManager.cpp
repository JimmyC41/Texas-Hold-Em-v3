#include "../include/TurnManager.h"
#include <algorithm>
#include <iostream>
using namespace std;

TurnManager::TurnManager() {}

void TurnManager::addPlayerInHand(const shared_ptr<Player>& player) {
    playersInHand.push_back(player);
}

void TurnManager::foldPlayerInHand(const shared_ptr<Player>& foldedPlayer) {
    auto it = find(playersInHand.begin(), playersInHand.end(), foldedPlayer);

    if (it != playersInHand.end()) {
        playersFolded.push_back(std::move(*it)); // Add the player to the folded players list
        playersInHand.erase(it); // Remove folded player from players in the hand
    }
}

void TurnManager::moveFoldedPlayersToInHand() {
    for (auto& player : playersFolded) {
            playersInHand.push_back(std::move(player));
    }
    playersFolded.clear();
}

void TurnManager::rotatePositions() {
    for (auto& player : playersInHand) {
        player->setPosition(getNextPosition(player->getPosition()));
    }
}

shared_ptr<Player> TurnManager::getBigBlindToAct() {
    for (auto& player : playersInHand) {
        if (player->getPosition() == Position::BIG_BLIND) {
            playerToAct = player;
            return playerToAct;
        }
    }

    cerr << "Error: Could not a player with the big blind!" << endl;
    return nullptr;
}

shared_ptr<Player> TurnManager::getEarlyPositionToAct() {
    for (int position = static_cast<int>(Position::SMALL_BLIND);
        position < static_cast<int>(Position::BUTTON);
        ++position) {

        auto player = getPlayerWithPosition(static_cast<Position>(position));
        if (player != nullptr) {
            playerToAct = player;
            return playerToAct;
        }
    }

    cerr << "Error: Could not find early position player first to act!" << endl;
    return nullptr;
}

shared_ptr<Player> TurnManager::getNextToAct() {
    Position curPosition = playerToAct->getPosition();

    for (int i = 0; i < NUM_POSITIONS; ++i) {
        curPosition = getNextPosition(curPosition);
        auto player = getPlayerWithPosition(static_cast<Position>(curPosition));
        if (player != nullptr) {
            playerToAct = player;
            return playerToAct;
        }
    }

    cerr << "Error: Could not find next player to act!" << endl;
    return nullptr;
}

void TurnManager::displayPlayerToAct() {
    cout << "Player to act is: " << playerToAct->getName() << endl;
}

// Helper Functions

shared_ptr<Player> TurnManager::getPlayerWithPosition(Position position) const {
    for (auto& player: playersInHand) {
        if (player->getPosition() == position) return player;
    }
    return nullptr;
}

Position TurnManager::getNextPosition(Position curPosition) const {
    return static_cast<Position>((static_cast<int>(curPosition) + 1) % NUM_POSITIONS);
}

int TurnManager::getNumPlayersInHand() const {
    return playersInHand.size();
}

int TurnManager::getNumPlayersFolded() const {
    return playersFolded.size();
}