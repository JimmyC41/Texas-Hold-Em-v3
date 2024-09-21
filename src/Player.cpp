#include "../include/Player.h"
#include <stdexcept>

Player::Player(std::string name, Position position, size_t chips)
    : name(name), position(position), chips(chips) {
    if (chips < 0) {
        throw invalid_argument("Player's starting chips cannot be negative.");
    }
}

// Setter Methods

void Player::setPosition(Position newPosition) {
    position = newPosition;
}

void Player::reduceChips(size_t amount) {
    validateChipAmount(amount);
    if (chips < amount) {
        throw runtime_error("Attempting to reduce player chips by more than their current chip count");
    }
    chips -= amount;
    cout << name << " chip count decreased by " << amount << " to " << chips << endl;
}

void Player::addChips(size_t amount) {
    validateChipAmount(amount);
    chips += amount;
    cout << name << " chip count increased by " << amount << " to " << chips << endl;
}

void Player::addHoleCard(const Card& card) {
    hand.push_back(card);
}

// Getter Methods

Position Player::getPosition() const {
    return position;
}

const std::vector<Card>& Player::getHand() const {
    return hand;
}

size_t Player::getChips() const {
    return chips;
}

void Player::resetHand() {
    hand.clear();
}

std::string Player::getName() const {
    return name;
}

// Helper Functions

// Private validation function to ensure valid chip operations
void Player::validateChipAmount(size_t amount) const {
    if (amount < 0) {
        throw invalid_argument("Chip modification amount cannot be negative!");
    }
}

// Returns the next position given a 9 handed table
Position Player::nextPosition(Position curPosition) {
    int newPosition = static_cast<int>(curPosition);
    newPosition = (newPosition + NUM_POSITIONS) % NUM_POSITIONS;
    return static_cast<Position>(newPosition);
}
