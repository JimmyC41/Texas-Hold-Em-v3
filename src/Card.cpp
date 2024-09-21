#include "../include/Card.h"

Card::Card() : suit(Suit::HEARTS), value(Value::TWO) {}

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

bool Card::operator<(const Card& other) const {
    if (value != other.value) {
        return value < other.value;
    }
    return suit < other.suit;
}

bool Card::operator==(const Card& other) const {
    return suit == other.suit && value == other.value;
}

// String representation of the card
string Card::toString() const {
    std::string valueStr;
    switch (value) {
        case Value::TWO: valueStr = "2"; break;
        case Value::THREE: valueStr = "3"; break;
        case Value::FOUR: valueStr = "4"; break;
        case Value::FIVE: valueStr = "5"; break;
        case Value::SIX: valueStr = "6"; break;
        case Value::SEVEN: valueStr = "7"; break;
        case Value::EIGHT: valueStr = "8"; break;
        case Value::NINE: valueStr = "9"; break;
        case Value::TEN: valueStr = "10"; break;
        case Value::JACK: valueStr = "Jack"; break;
        case Value::QUEEN: valueStr = "Queen"; break;
        case Value::KING: valueStr = "King"; break;
        case Value::ACE: valueStr = "Ace"; break;
    }

    std::string suitStr;
    switch (suit) {
        case Suit::HEARTS: suitStr = "Hearts"; break;
        case Suit::DIAMONDS: suitStr = "Diamonds"; break;
        case Suit::CLUBS: suitStr = "Clubs"; break;
        case Suit::SPADES: suitStr = "Spades"; break;
    }

    return valueStr + " of " + suitStr;
}