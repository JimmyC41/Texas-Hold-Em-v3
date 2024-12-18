#include "../include/HandEvaluator.h"

// PokerHand Struct

PokerHand::PokerHand() : bitwise(0), handSize(0), category(HandCategory::NONE) {}

// HandEvaluator Class

HandEvaluator::HandEvaluator() : playerHands() {}

vector<shared_ptr<Player>> HandEvaluator::getSortedPlayers() {
    vector<shared_ptr<Player>> players;
    for (const auto& entry : playerHands) players.push_back(entry.first);

    sort(players.begin(), players.end(), [this](const shared_ptr<Player>& a, const shared_ptr<Player>& b) {
        return compareHands(playerHands.at(a), playerHands.at(b));
    });

    return players;
}

void HandEvaluator::addDealtCard(shared_ptr<Player> player, const Card& card) {
    PokerHand& hand = playerHands[player];
    assert(hand.handSize <= MAX_HAND_SIZE);

    // Increment hand size
    hand.handSize += 1;

    // Add card to hand and sort in decreasing order of values
    hand.hand.push_back(card);
    sort(hand.hand.begin(), hand.hand.end(), [](const Card& a, const Card& b) {
        return a.getValue() > b.getValue();
    });

    // Update bitmask representation of hand
    hand.bitwise |= card.getBitMask();
}

unordered_map<shared_ptr<Player>, PokerHand>& HandEvaluator::getPlayerHandsMap() {
    return playerHands;
}

void HandEvaluator::clearHandEvaluator() {
    for (auto& [player, pokerHand] : playerHands) {
        pokerHand.hand.clear();
        pokerHand.bestFiveCards.clear();
        pokerHand.bitwise = 0;
        pokerHand.category = HandCategory::NONE;
        pokerHand.handSize = 0;
    }
    
    playerHands.clear();
}

void HandEvaluator::evaluatePlayerHands() {
    for (auto& [player, pokerHand] : playerHands) {
        evaluateHand(pokerHand);
    }
}

bool HandEvaluator::compareHands(const PokerHand& handA, const PokerHand& handB) {
    // Compare by hand category first (higher is stronger)
    if (handA.category != handB.category) {
        return handA.category > handB.category;
    }

    // Compare best five cards lexicographically
    for (size_t i = 0; i < handA.bestFiveCards.size(); ++i) {
        if (handA.bestFiveCards[i].getValue() != handB.bestFiveCards[i].getValue()) {
            return handA.bestFiveCards[i].getValue() > handB.bestFiveCards[i].getValue();
        }
    }

    return false;
}

void HandEvaluator::evaluateHand(PokerHand& hand) {
    if (hand.handSize < MIN_HAND_SIZE) {
        hand.category = NONE;
        return;
    }
    else if (isRoyalFlush(hand)) return;
    else if (isStraightFlush(hand)) return;
    else if (isNOfAKind(hand, 4)) return;
    else if (isFullHouse(hand)) return;
    else if (isFlush(hand)) return;
    else if (isStraight(hand)) return;
    else if (isNOfAKind(hand, 3)) return;
    else if (isTwoPair(hand)) return;
    else if (isNOfAKind(hand, 2)) return;
    else if (isHighCard(hand)) return;
}

void HandEvaluator::printPlayerHands() const {
    std::cout << "Player Hands Information:\n";
    for (const auto& [player, pokerHand] : playerHands) {
        // Assuming Player has a method getName() for player identification
        std::cout << "Player: " << player->getName() << endl;
        std::cout << "  Hand Category: ";

        // Print the category of the hand
        switch (pokerHand.category) {
            case NONE:            std::cout << "None"; break;
            case HIGH_CARD:       std::cout << "High Card"; break;
            case ONE_PAIR:        std::cout << "One Pair"; break;
            case TWO_PAIR:        std::cout << "Two Pair"; break;
            case THREE_OF_A_KIND: std::cout << "Three of a Kind"; break;
            case STRAIGHT:        std::cout << "Straight"; break;
            case FLUSH:           std::cout << "Flush"; break;
            case FULL_HOUSE:      std::cout << "Full House"; break;
            case FOUR_OF_A_KIND:  std::cout << "Four of a Kind"; break;
            case STRAIGHT_FLUSH:  std::cout << "Straight Flush"; break;
            case ROYAL_FLUSH:     std::cout << "Royal Flush"; break;
        }
        std::cout << '\n';

        // Print hand size
        std::cout << "  Hand Size: " << pokerHand.handSize << '\n';

        // Print cards in the hand
        std::cout << "  Cards: ";
        for (const auto& card : pokerHand.hand) {
            std::cout << card.toString() << ", ";
        }
        std::cout << '\n';

        // Print best 5 cards
        std::cout << "  Best 5 Cards: ";
        for (const auto& card : pokerHand.bestFiveCards) {
            std::cout << card.toString() << ", ";
        }
        std::cout << '\n';

        // Print the bitwise representation
        std::cout << "  Bitwise Representation: " << std::bitset<64>(pokerHand.bitwise) << '\n';

        std::cout << "----------------------------------\n";
    }
}

// Helper functions for evaluating hand categories and the best 5 card combination

bool HandEvaluator::isRoyalFlush(PokerHand& hand) {
    for (int suit = static_cast<int>(Suit::HEARTS); suit <= static_cast<int>(Suit::SPADES); ++suit) {
        uint64_t suitMask = getSuitMask(hand.bitwise, suit);
        if ((suitMask & BITMASK_ACE_STRAIGHT) == BITMASK_ACE_STRAIGHT) {
            hand.category = ROYAL_FLUSH;
            findStraight(hand, Value::ACE, true, static_cast<Suit>(suit));
            return true;
        }
    }
    return false;
}

bool HandEvaluator::isStraightFlush(PokerHand& hand) {
    for (int suit = static_cast<int>(Suit::HEARTS); suit <= static_cast<int>(Suit::SPADES); ++suit) {
        uint64_t suitMask = getSuitMask(hand.bitwise, suit);
        for (const uint64_t mask : straightMasks) {
            if ((suitMask & mask) == mask) {
                hand.category = STRAIGHT_FLUSH;
                findStraight(hand, straightMaskToHighCard(mask), true, static_cast<Suit>(suit));
                return true;
            }
        }
    }
    return false;
}

bool HandEvaluator::isNOfAKind(PokerHand& hand, int n) {
    for (int value = static_cast<int>(Value::TWO); value <= static_cast<int>(Value::ACE); value++) {
        if (countBitsForValue(hand.bitwise, value) == n) {
            switch (n) {
                case 2:
                    hand.category = ONE_PAIR;
                    findOnePair(hand, static_cast<Value>(value));
                    break;
                case 3:
                    hand.category = THREE_OF_A_KIND;
                    findThreeOfAKind(hand, static_cast<Value>(value));
                    break;
                case 4:
                    hand.category = FOUR_OF_A_KIND;
                    findFourOfAKind(hand, static_cast<Value>(value));
                    break;
            }
            return true;
        }
    }
    return false;
}

bool HandEvaluator::isFullHouse(PokerHand& hand) {
    Value trips = static_cast<Value>(-1);
    Value pair = static_cast<Value>(-1);

    for (int value = static_cast<int>(Value::TWO); value <= static_cast<int>(Value::ACE); value++) {
        if (countBitsForValue(hand.bitwise, value) == 3) trips = static_cast<Value>(value);
        else if (countBitsForValue(hand.bitwise, value) == 2) pair = static_cast<Value>(value);
    }

    if (trips != static_cast<Value>(-1) && pair != static_cast<Value>(-1)) {
        hand.category = FULL_HOUSE;
        findFullHouse(hand, trips, pair);
        return true;
    } else {
        return false;
    }
}

bool HandEvaluator::isFlush(PokerHand& hand) {
    for (int suit = static_cast<int>(Suit::HEARTS); suit <= static_cast<int>(Suit::SPADES); ++suit) {
        uint64_t suitMask = getSuitMask(hand.bitwise, suit);
        if (countSetBits(suitMask) >= MIN_HAND_SIZE) {
            hand.category = FLUSH;
            findFlush(hand, static_cast<Suit>(suit));
            return true;
        }
    }
    return false;
}

bool HandEvaluator::isStraight(PokerHand& hand) {
    uint64_t handNoSuits = getAllSuitsMask(hand.bitwise);
    for (const uint64_t mask : straightMasks) {
        if ((handNoSuits & mask) == mask) {
            hand.category = STRAIGHT;
            findStraight(hand, straightMaskToHighCard(mask), false, Suit::HEARTS);
            return true;
        }
    }
    return false;
}

bool HandEvaluator::isTwoPair(PokerHand& hand) {
    Value pairOne = static_cast<Value>(-1);
    Value pairTwo = static_cast<Value>(-1);

    for (int value = static_cast<int>(Value::TWO); value <= static_cast<int>(Value::ACE); value++) {
        if (countBitsForValue(hand.bitwise, value) == 2) {
            if (pairOne == static_cast<Value>(-1)) pairOne = static_cast<Value>(value);
            else pairTwo = static_cast<Value>(value);
        }
    }
    
    if (pairOne != static_cast<Value>(-1) && pairTwo != static_cast<Value>(-1)) {
        hand.category = TWO_PAIR;
        findTwoPair(hand, pairOne, pairTwo);
        return true;
    } else {
        return false;
    }
}

bool HandEvaluator::isHighCard(PokerHand& hand) {
    hand.category = HIGH_CARD;
    findHighCard(hand);
    return true;
}

// Helper methods to find the best 5 card combination

// Method to find royal flush, straight flush and normal straights
void HandEvaluator::findStraight(PokerHand& hand, Value highCard, bool isFlush, Suit flushSuit) {
    bool isFiveHighStraight = false;
    
    // Start from the high card of the straight and move to the end
    Value curVal = highCard;
    Value endVal = static_cast<Value>(static_cast<int>(highCard) - 4);

    // For the 5-high straight, add the {5, 4, 3, 2} part of the straight, then the A
    if (highCard == Value::FIVE) {
        isFiveHighStraight = true;
        endVal = Value::TWO;
    }

    // Iterate through the hand and add the straight cards in decreasing order
    for (const Card& card : hand.hand) {
        if ((!isFlush && card.getValue() == static_cast<Value>(curVal)) ||
            (isFlush && card.getValue() == static_cast<Value>(curVal) && card.getSuit() == flushSuit)) {
            
            hand.bestFiveCards.push_back(card);

            // When we find the last card of the straight, exit early
            if (curVal == endVal) break;

            // Decrement the current value we are looking for
            curVal = static_cast<Value>(static_cast<int>(curVal) - 1);
        }
    }

    // Edge case for the 5-high straight where we need to find the A and append it to the end
    if (isFiveHighStraight) {
        for (const Card& card : hand.hand) {
            if ((!isFlush && card.getValue() == Value::ACE) ||
                (isFlush && card.getValue() == Value::ACE && card.getSuit() == flushSuit)) {
                    hand.bestFiveCards.push_back(card);
                    break;
                }
        }
    }
}

void HandEvaluator::findFourOfAKind(PokerHand& hand, Value quads) {
    cout << "Quad Value is " << static_cast<int>(quads) << endl;
    // First, find the four of a kind
    for (const Card& card : hand.hand) {
        if (card.getValue() == quads) hand.bestFiveCards.push_back(card);
    }

    // Then, find the high card kicker
    for (const Card& card : hand.hand) {
        if (card.getValue() != quads) {
            hand.bestFiveCards.push_back(card);
            break;
        }
    }

}

void HandEvaluator::findFullHouse(PokerHand& hand, Value trips, Value pair) {
    // First, find the three of a kind
    for (const Card& card : hand.hand) {
        if (card.getValue() == trips) hand.bestFiveCards.push_back(card);
    }

    // Then, find the pair
    for (const Card& card : hand.hand) {
        if (card.getValue() == pair) hand.bestFiveCards.push_back(card);
    }

}
void HandEvaluator::findFlush(PokerHand& hand, Suit suit) {
    // Iterate through the hand and add the first 5 cards
    for (const Card& card : hand.hand) {
        if (card.getSuit() == suit) hand.bestFiveCards.push_back(card);
        if (hand.bestFiveCards.size() == 5) break;
    }

}

void HandEvaluator::findThreeOfAKind(PokerHand& hand, Value trips) {
    // First, find the three of a kind
    for (const Card& card : hand.hand) {
        if (card.getValue() == trips) hand.bestFiveCards.push_back(card);
    }

    // Then, add the high card kickers
    int numKickers = 0;
    for (const Card& card : hand.hand) {
        if (card.getValue() != trips) {
            hand.bestFiveCards.push_back(card);
            numKickers++;
            if (numKickers == 2) break;
        }
    }
}
void HandEvaluator::findTwoPair(PokerHand& hand, Value pairOne, Value pairTwo) {
    // First, find pairTwo
    for (const Card& card : hand.hand) {
        if (card.getValue() == pairTwo) hand.bestFiveCards.push_back(card);
    }

    // Then, find pairOne
    for (const Card& card : hand.hand) {
        if (card.getValue() == pairOne) hand.bestFiveCards.push_back(card);
    }

    // Then, add the high card kicker
    for (const Card& card : hand.hand) {
        if (card.getValue() != pairOne && card.getValue() != pairTwo) {
            hand.bestFiveCards.push_back(card);
            break;
        }
    }
}

void HandEvaluator::findOnePair(PokerHand& hand, Value pair) {
    // First, find the pair
    for (const Card& card : hand.hand) {
        if (card.getValue() == pair) hand.bestFiveCards.push_back(card);
    }

    // Then, add the high card kickers
    int numKickers = 0;
    for (const Card& card : hand.hand) {
        if (card.getValue() != pair) {
            hand.bestFiveCards.push_back(card);
            numKickers++;
            if (numKickers == 3) break;
        }
    }
}

void HandEvaluator::findHighCard(PokerHand& hand) {
    // Add the first 5 high cards from the hand
    int numCards = 0;
    for (const Card& card : hand.hand) {
        hand.bestFiveCards.push_back(card);
        numCards++;
        if (numCards == 5) break;
    }
}

// Helper methods for bitwise operations

uint64_t HandEvaluator::getAllSuitsMask(uint64_t hand) {
    return  (hand & BITMASK_13_BITS) |
            ((hand >> 13) & BITMASK_13_BITS) |
            ((hand >> 26) & BITMASK_13_BITS) |
            ((hand >> 39) & BITMASK_13_BITS);
}

uint64_t HandEvaluator::getSuitMask(uint64_t hand, int suit) {
    return (hand >> NUM_VALUES * suit) & BITMASK_13_BITS;
}

int HandEvaluator::countSetBits(uint64_t mask) {
    int count = 0;
    while (mask) {
        mask &= (mask - 1);
        count++;
    }
    return count;
}

int HandEvaluator::countBitsForValue(uint64_t hand, int value) {
    int valueFreq = 0;
    uint64_t valueMask = 1ULL << (value - 2);
    for (int suit = static_cast<int>(Suit::HEARTS); suit <= static_cast<int>(Suit::SPADES); ++suit) {
        uint64_t suitMask = getSuitMask(hand, suit);
        if ((suitMask & valueMask) == valueMask) valueFreq++;
    }
    return valueFreq;
}

Value HandEvaluator::straightMaskToHighCard(uint64_t straightMask) {
    switch (straightMask) {
        case 0x100F: return Value::FIVE;
        case 0x1F: return Value::SIX;
        case 0x3E: return Value::SEVEN;
        case 0x7C: return Value::EIGHT;
        case 0xF8: return Value::NINE;
        case 0x1F0: return Value::TEN;
        case 0x3E0: return Value::JACK;
        case 0x7C0: return Value::QUEEN;
        case 0xF80: return Value::KING;
        case 0x1F00:return Value::ACE;
        default: return Value::TWO;
    }
}