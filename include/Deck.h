#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <array>
#include <stdexcept>
using namespace std;

const int DECK_SIZE = 52;

class Deck {
private:
    array<Card, DECK_SIZE> deck;
    size_t deckIndex;
    bool isShuffled;
    void shuffleDeck();
public:
    Deck();
    Card& dealCard();
    void burnCard();
    void resetDeck();
};

#endif // DECK_H