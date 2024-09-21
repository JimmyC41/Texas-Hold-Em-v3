#include "../include/Card.h"

// Default constructor with constexpr initialisation
constexpr Card::Card() : suit(Suit::HEARTS), value(Value::TWO) {}

// Constructor with validation for suit and value
Card::Card(Suit suit, Value value) : suit(suit), value(value) {
    validate();
}

// Accessor method for suit
Suit Card::getSuit() const {
    return suit;
}

// Accessor method for value
Value Card::getValue() const {
    return value;
}

// Validates suits and values
void Card::validate() const {
    if (static_cast<int>(suit) < 0 || static_cast<int>(suit) > 3) {
        throw invalid_argument("Attempting to construct a Card: Invalid suit!");
    }
    if (static_cast<int>(value) < 2 || static_cast<int>(value) > 14) {
        throw invalid_argument("Attempting to construct a Card: Invalid value!");
    }
}

// Operator overload to compare equality
bool Card::operator==(const Card& other) const {
    if (value == other.value) {
        suit < other.suit;
    }
    return value < other.value;
}