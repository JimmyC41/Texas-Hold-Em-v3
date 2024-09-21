#include "../include/Dealer.h"
#include <iostream>

Dealer::Dealer(Deck& deck, Board& board) : deck(deck), board(board) {}

Dealer Dealer::createDealer(Deck& deck, Board& board) {
    return Dealer(deck, board);
}

void Dealer::dealPlayer(Player& player) {
    for (int i = 0; i < 2; ++i) {
        Card& holeCard = deck.dealCard();
        player.addHoleCard(holeCard);
        cout << player.getName() << " has been dealt " << holeCard.toString() << endl;
    }
}

void Dealer::resetDeck() {
    deck.resetDeck();
}

void Dealer::dealBoard(int numCards, bool burnCard) {
    if (burnCard) {
        deck.burnCard();
    }
    for (int i = 0; i < numCards; ++i) {
        Card& communityCard = deck.dealCard();
        board.addCommunityCard(communityCard);
    }
}

size_t Dealer::getRemainingDeckSize() const {
    return DECK_SIZE - deck.getDealtCardCount();
}

void Dealer::printDeckState() const {
    cout << "Deck contains " << getRemainingDeckSize() << " cards." << endl;
}