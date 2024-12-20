#ifndef CARD_H
#define CARD_H

#include <stdexcept>
#include <string>
using namespace std;

const int NUM_VALUES = 13;

enum class Suit {
    HEARTS = 0,
    DIAMONDS = 1,
    CLUBS = 2,
    SPADES = 3
};

enum class Value {
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    JACK = 11,
    QUEEN = 12,
    KING = 13,
    ACE = 14
};

class Card {
private:
    Suit suit;
    Value value;
    void validate() const;
public:
    Card();
    explicit Card(Suit suit, Value value);
    Suit getSuit() const;
    Value getValue() const;
    uint64_t getBitMask() const;
    bool operator==(const Card& other) const;
    bool operator<(const Card& other) const;
    string toString() const;
};

#endif // CARD_H