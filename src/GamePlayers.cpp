#include "../include/GamePlayers.h"
#include "../include/Player.h"

GamePlayers::GamePlayers() {}

shared_ptr<Player> GamePlayers::addPlayerToGame(const string& name, size_t chips) {
    if (gamePlayers.size() == MAX_NUM_PLAYERS) {
        throw runtime_error("Table is full. Unable to add another player.");
    }
    auto player = make_shared<Player>(name, getEmptySeatPosition(), chips);
    gamePlayers.push_back(player);

    // Sort players by their position
    sortGamePlayers();

    cout << "Player " << name << " added to the game!" << endl;
    return player;
}

void GamePlayers::removePlayerFromGame(const string& playerName) {
    if (gamePlayers.size() <= MIN_NUM_PLAYERS) {
        throw runtime_error("Removing this player would result in fewer than the minimum required number of players.");
    }

    auto it = remove_if(gamePlayers.begin(), gamePlayers.end(),
                        [playerName](const shared_ptr<Player>& player) {
                            return player->getName() == playerName;
                        });
    
    if (it != gamePlayers.end()) {
        gamePlayers.erase(it, gamePlayers.end());
        cout << "Player " << playerName << " removed from the game!" << endl;
    } else {
        cout << "Player " << playerName << " could not be found for removal!" << endl;
    }
}

void GamePlayers::removeAllPlayersInGame() {
    gamePlayers.clear();
}

bool GamePlayers::isEnoughPlayersInGame() {
    if (gamePlayers.size() < 2) {
        cout << "Not enough players to start the game!" << endl;
        return false;
    }
    return true;
}

int GamePlayers::getNumPlayersInGame() const {
    return gamePlayers.size();
}

void GamePlayers::displayPlayersInGame() const {
    cout << "Displaying player names and chips!" << endl;
    for (auto& player : gamePlayers) {
        cout << "Name: " << player->getName() << " | Chips: " << player->getChips() 
            << " | Position: " << Player::positionToStr(player->getPosition()) << endl;
    }
}

// Helper Functions

void GamePlayers::sortGamePlayers() {
    sort(gamePlayers.begin(), gamePlayers.end(),
        [](const shared_ptr<Player>& a, const shared_ptr<Player>& b) {
            return *a < *b;
        });
}

bool GamePlayers::isEmptySeatInPosition(Position position) const {
    auto it = find_if(gamePlayers.begin(), gamePlayers.end(),
                    [position](const shared_ptr<Player>& player) {
                        return player->getPosition() == position;
                    });
    if (it == gamePlayers.end()) return true;
    return false;
}

Position GamePlayers::getEmptySeatPosition() const {

    // First two seats to be filled should be big blind and button respectively
    if (gamePlayers.size() == 0) return Position::BIG_BLIND;
    else if (gamePlayers.size() == 1) return Position::DEALER;

    for (int position = static_cast<int>(Position::SMALL_BLIND);
        position < static_cast<int>(Position::DEALER);
        ++position) {

        Position curPosition = static_cast<Position>(position);
        if (isEmptySeatInPosition(static_cast<Position>(curPosition))) {
            return curPosition;
        }
    }
    throw runtime_error("No empty positions available.");
}

shared_ptr<Player> GamePlayers::getPlayerWithPosition(Position position) {
    auto it = find_if(gamePlayers.begin(), gamePlayers.end(),
                    [position](const shared_ptr<Player>& player) {
                        return player->getPosition() == position;
                    });
    
    if (it != gamePlayers.end()) return (*it);
    else return nullptr;
}