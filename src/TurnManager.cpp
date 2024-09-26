#include "../include/TurnManager.h"
#include <algorithm>
#include <iostream>
using namespace std;

TurnManager::TurnManager() {}

void TurnManager::addPlayerInHand(const shared_ptr<Player>& player) {
    playersInHand.push_back(player);
    sortPlayersAndSetButton();
}

void TurnManager::foldPlayerInHand(const shared_ptr<Player>& foldedPlayer) {
    auto it = find(playersInHand.begin(), playersInHand.end(), foldedPlayer);

    if (it != playersInHand.end()) {
        playersFolded.push_back(std::move(*it)); // Add the player to the folded players list
        playersInHand.erase(it); // Remove folded player from players in the hand
    }
    sortPlayersAndSetButton();
}

void TurnManager::moveFoldedPlayersToInHand() {
    for (auto& player : playersFolded) {
            playersInHand.push_back(std::move(player));
    }
    playersFolded.clear();
    sortPlayersAndSetButton();
}

void TurnManager::rotatePositions() {
    int numPlayers = getNumPlayersInHand();
    if (numPlayers < 2) return;

    // Heads up edge case
    if (numPlayers == 2) {
        playersInHand[0]->setPosition(Position::BIG_BLIND);
        playersInHand[1]->setPosition(Position::SMALL_BLIND);
        sortPlayersAndSetButton();
        return;
    }

    // 3 or more players
    for (auto& curPlayer : playersInHand) {
        // Set player position one position clockwise
        curPlayer->setPosition(getNextPosition(curPlayer->getPosition()));

        // If the player has button, they become the small blind regardless of next position
        if (curPlayer == playerWithButton) curPlayer->setPosition(Position::SMALL_BLIND);
    }
    sortPlayersAndSetButton();
}

shared_ptr<Player> TurnManager::getBigBlindToAct() {
    auto player = getPlayerWithPosition(Position::BIG_BLIND);
    if (player == nullptr) cerr << "Error: Could not find a player with the big blind!" << endl;
    playerToAct = player;
    return playerToAct;
}

shared_ptr<Player> TurnManager::getSmallBlindToAct() {
    auto player = getPlayerWithPosition(Position::SMALL_BLIND);
    if (player == nullptr) cerr << "Error: Could not find a player with the small blind!" << endl;
    playerToAct = player;
    return playerToAct;
}

shared_ptr<Player> TurnManager::getEarlyPositionToAct() {
    if (playersInHand.size() == 0) {
        cerr << "Error: Could not find early position player first to act!" << endl;
        return nullptr;
    }
    return playersInHand.front();
}

shared_ptr<Player> TurnManager::getNextToAct() {
    auto it = find(playersInHand.begin(), playersInHand.end(), playerToAct);

    if (it != playersInHand.end()) {
        auto nextIt = it + 1;
        if (nextIt == playersInHand.end()) {
            nextIt = playersInHand.begin();
        }

        if (playerToAct) {
            playerToAct = (*nextIt);
            return playerToAct;
        }
    }

    cerr << "Error: Could not find next player to act!" << endl;
    return nullptr;
}

shared_ptr<Player> TurnManager::getPlayerWithButton() const {
    return playerWithButton;
}

void TurnManager::displayPlayerToAct() const {
    cout << "Player to act is: " << playerToAct->getName() << endl;
}

void TurnManager::displayPlayerWithButton() const {
    cout << "Player with button is: " << playerWithButton->getName() << endl;
}

void TurnManager::displayPlayersInHand() const {
    cout << "Displaying players in hand: " << endl;
    for (auto& player : playersInHand) {
        cout << "   Player: " << player->getName() << " | Position: " << Player::positionToStr(player->getPosition()) << endl;
    }
    cout << "   ----------FINISHED----------" << endl;
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

void TurnManager::sortPlayersAndSetButton() {
    // Sort players
    sort(playersInHand.begin(), playersInHand.end(),
        [](const shared_ptr<Player>& a, const shared_ptr<Player>& b) {
            return *a < *b;
        });
    // Set button
    if (playersInHand.size() == 2) {
        playerWithButton = playersInHand.front();
    } else {
        playerWithButton = playersInHand.back();
    }
}