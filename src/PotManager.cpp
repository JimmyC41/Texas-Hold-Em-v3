#include "../include/PotManager.h"
#include <limits>
#include <iostream>

// Helper Functions

void PotManager::newPot() {
    pots.emplace_back();
}

Pot& PotManager::getCurPot() {
    if (pots.empty()) throw runtime_error("No pots available to fetch!");
    return pots.back();
}

bool PotManager::allPotsCreated() const {
    for (auto& [player, betInfo] : playerBets) {
        if (betInfo.betSize != 0) return false;
    }
    return true;
}

size_t PotManager::findMinBet() const { 
    size_t minBet = numeric_limits<size_t>::max();
    for (auto& [player, betInfo] : playerBets) {
        if (betInfo.betSize == 0) continue; // minBet cannot be 0
        if (betInfo.betSize < minBet)  minBet = betInfo.betSize;
    }
    return minBet;
}

void PotManager::displayPlayerBets() {
    if (playerBets.empty()) {
        cout << "playerBets map is empty!" << endl;
        return;
    }
    
    for (const auto& [player, betInfo] : playerBets) {
        cout << "Player: " << player->getName() << " | Bet: " << betInfo.betSize << endl;
    }
}

void PotManager::displayPots() {
    for (size_t i = 0; i < pots.size(); ++i) {
        const Pot& pot = pots[i];

        cout << "Pot " << i + 1 << ": " << pot.getChips() << " chips" << endl;
        if (pot.getEligiblePlayers().empty()) {
            cout << "No players in this pot!" << endl;
        } else {
            cout << "The following players are eligible to Pot " << i + 1 << endl;
            for (const auto& player : pot.getEligiblePlayers()) {
                cout << "Player: " << player->getName() << endl;
            }
        }

    }
}

// Pot Manager

void PotManager::resetPots() {
    pots.clear();
    deadChips = 0;
    newPot();
}

PotManager::PotManager() : deadChips(0) {
    newPot();
}

void PotManager::addPlayerBet(const shared_ptr<Player>& player, size_t bet, bool isAllIn) {
    auto it = playerBets.find(player);

    // Add player and their bet if they don't exist
    if (it == playerBets.end()) {
        playerBets[player] = {bet, isAllIn};
        player->reduceChips(bet);
    }
    // If player already exists, simply update their recent bet
    else { 
        size_t previousBet = it->second.betSize;
        size_t amountToMatch = bet - previousBet;

        it->second.betSize = bet;
        it->second.isAllIn = isAllIn;

        player->reduceChips(amountToMatch);
    }
}

size_t PotManager::getRecentBet(const shared_ptr<Player>& player) {
    auto it = playerBets.find(player);
    if (it != playerBets.end()) {
        return it->second.betSize;
    }
    return 0;
}

void PotManager::foldPlayerBet(const shared_ptr<Player>& player) {
    auto it = playerBets.find(player);

    // If player has not made a bet, exit early.
    // No need to change deadMoney or playerBets.
    if (it == playerBets.end()) return;
    
    // Increment dead money in pot
    deadChips += playerBets[player].betSize;

    // Update folded player's recent bet to 0
    playerBets[player].betSize = 0;
}

void PotManager::calculatePots() {
    // Suppose playerBets is populated with player's and their bets at the end of a street.

    while (!allPotsCreated()) {
        size_t minBet = findMinBet();
        Pot& curPot = getCurPot();

        // Add each player's contribution to the current pot
        for (auto& [player, betInfo] : playerBets) {
            if (betInfo.betSize == 0) continue;
            if (!curPot.isPlayerInPot(player)) curPot.addPlayer(player);
            
            playerBets[player].betSize -= minBet;
            curPot.addChips(minBet);
        }

        // Add dead money to the current pot
        if (deadChips > minBet) {
            curPot.addChips(minBet);
            deadChips -= minBet;
        } else {
            curPot.addChips(deadChips);
            deadChips = 0;
        }

        // Create a side pot if required
        if (!allPotsCreated()) newPot();
    }
}

void PotManager::resetPlayerBets() {
    for (auto& [player, betInfo] : playerBets) {
        playerBets[player].betSize = 0;
    }
}

const Pot& PotManager::getPot(int index) const {
    if (index >= pots.size()) throw out_of_range("Invalid pot index: " + to_string(index));
    return pots[index];
}

int PotManager::getNumPots() const {
    return pots.size();
}


// Pot Structure

Pot::Pot() : chips(0) {}

void Pot::addPlayer(const shared_ptr<Player>& player) {
    eligiblePlayers.push_back(player);
}

void Pot::addChips(size_t amount) {
    chips += amount;
}

size_t Pot::getChips() const {
    return chips;
}

const vector<shared_ptr<Player>>& Pot::getEligiblePlayers() const {
    return eligiblePlayers;
}

bool Pot::isPlayerInPot(const shared_ptr<Player>& player) {
    return find(eligiblePlayers.begin(), eligiblePlayers.end(), player) != eligiblePlayers.end();
}