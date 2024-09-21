#include "../include/Board.h"
#include <iostream>

Board::Board() {}

void Board::addCommunityCard(const Card& card) {
    validateAddition();
    communityCards.push_back(card);
}

void Board::clearBoard() {
    communityCards.clear();
}

void Board::validateAddition() const {
    if (communityCards.size() >= 5) {
        throw runtime_error("Cannot add more than 5 community cards.");
    }
}

// String representation of the board's community cards
void Board::printBoardState() const {
    string state = "Board: ";
    for (const auto& card : communityCards) {
        state += card.toString() + " ";
    }
    if (!state.empty()) cout << state << endl;
    else cout << "No cards on the board!" << endl;
}

bool Board::isFlopDealt() const {
    return communityCards.size() == 3;
}

bool Board::isTurnDealt() const {
    return communityCards.size() == 4;
}

bool Board::isRiverDealt() const {
    return communityCards.size() == 5;
}

size_t Board::getCommunityCardCount() const {
    return communityCards.size();
}