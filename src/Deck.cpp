#include "../include/Deck.h"
#include <algorithm>
#include <random>
#include <assert.h>
using namespace std;

Deck::Deck() : deckIndex(0), isShuffled(false) {
    size_t index = 0;
    for (int suit = 0; suit < 4; ++suit) {
        for (int value = 2; value <= 14; ++value) {
            deck[index++] = Card(static_cast<Suit>(suit), static_cast<Value>(value));
        }
    }
    shuffleDeck();
}

Card& Deck::dealCard() {
    if (!isShuffled) {
        throw runtime_error("Deck must be shuffled before dealing cards!");
    }
    if (deckIndex >= DECK_SIZE) {
        throw out_of_range("No more cards left to deal!");
    }
    return deck[deckIndex++];
}

void Deck::burnCard() {
    if (!isShuffled) {
        throw runtime_error("Deck must be shuffled before burning cards!");
    }
    if (deckIndex >= DECK_SIZE) {
        throw out_of_range("No more cards left to burn!");
    }
    deckIndex++;
}

void Deck::resetDeck() {
    deckIndex = 0;
    shuffleDeck();
}

void Deck::shuffleDeck() {
    random_device rd;
    mt19937 g(rd());
    shuffle(deck.begin(), deck.end(), g);
    deckIndex = 0;
    isShuffled = true;
}