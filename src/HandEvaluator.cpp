#include "../include/HandEvaluator.h"

// PokerHand Struct

PokerHand::PokerHand() : bitwise(0), handSize(0), category(HandCategory::NONE), bestFiveCards() {}

// HandEvaluator Class

HandEvaluator::HandEvaluator() : playerHands() {}

void HandEvaluator::addDealtCard(shared_ptr<Player> player, const Card& card) {
    PokerHand& p = playerHands[player];
    assert(p.handSize <= MAX_HAND_SIZE);

    // Increment hand size
    p.handSize += 1;

    // Add card to hand and sort in decreasing order of values
    p.hand.push_back(card);
    sort(p.hand.begin(), p.hand.end(), [](const Card& a, const Card& b) {
        return a.getValue() > b.getValue();
    });

    // Update bitmask representation of hand
    p.bitwise |= card.getBitMask();
}

void HandEvaluator::evaluatePlayerHands() {
    for (const auto& [player, handInfo] : playerHands) {

    }
}

vector<shared_ptr<Player>> HandEvaluator::getPlayerHandRanking() {
    
}

void HandEvaluator::resetPlayerHands() {

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

// Helper functions for evaluating hand categories and the best 5 card combination

bool HandEvaluator::isRoyalFlush(PokerHand& hand) {
    for (int suit = static_cast<int>(Suit::HEARTS); suit <= static_cast<int>(Suit::SPADES); ++suit) {
        uint64_t suitMask = getSuitMask(hand.bitwise, suit);
        if ((suitMask && BITMASK_ACE_STRAIGHT) == BITMASK_ACE_STRAIGHT) {
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
                    findOnePair(hand, static_cast<Value>(value));
                    break;
                case 3:
                    findThreeOfAKind(hand, static_cast<Value>(value));
                    break;
                case 4:
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
void findStraight(PokerHand& hand, Value highCard, bool isFlush, Suit flushSuit) {
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
            
            hand.bestFiveCards.emplace_back(card);

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
                    hand.bestFiveCards.emplace_back(card);
                    break;
                }
        }
    }
}

void findFourOfAKind(PokerHand& hand, Value quads) {
    // First, find the four of a kind
    for (const Card& card : hand.hand) {
        if (card.getValue() == quads) hand.bestFiveCards.emplace_back(card);
    }

    // Then, find the high card kicker
    for (const Card& card : hand.hand) {
        if (card.getValue() != quads) {
            hand.bestFiveCards.emplace_back(card);
            break;
        }
    }

}

void findFullHouse(PokerHand& hand, Value trips, Value pair) {
    // First, find the three of a kind
    for (const Card& card : hand.hand) {
        if (card.getValue() == trips) hand.bestFiveCards.emplace_back(card);
    }

    // Then, find the pair
    for (const Card& card : hand.hand) {
        if (card.getValue() == pair) hand.bestFiveCards.emplace_back(card);
    }

}
void findFlush(PokerHand& hand, Suit suit) {
    // Iterate through the hand and add the first 5 cards
    for (const Card& card : hand.hand) {
        if (card.getSuit() == suit) hand.bestFiveCards.emplace_back(card);
        if (hand.bestFiveCards.size() == 5) break;
    }

}

void findThreeOfAKind(PokerHand& hand, Value trips) {
    // First, find the three of a kind
    for (const Card& card : hand.hand) {
        if (card.getValue() == trips) hand.bestFiveCards.emplace_back(card);
    }

    // Then, add the high card kickers
    int numKickers = 0;
    for (const Card& card : hand.hand) {
        if (card.getValue() != trips) {
            hand.bestFiveCards.emplace_back(card);
            numKickers++;
            if (numKickers == 2) break;
        }
    }
}
void findTwoPair(PokerHand& hand, Value pairOne, Value pairTwo) {
    // First, find pairTwo
    for (const Card& card : hand.hand) {
        if (card.getValue() == pairTwo) hand.bestFiveCards.emplace_back(card);
    }

    // Then, find pairOne
    for (const Card& card : hand.hand) {
        if (card.getValue() == pairOne) hand.bestFiveCards.emplace_back(card);
    }

    // Then, add the high card kicker
    for (const Card& card : hand.hand) {
        if (card.getValue() != pairOne && card.getValue() != pairTwo) {
            hand.bestFiveCards.emplace_back(card);
            break;
        }
    }
}

void findOnePair(PokerHand& hand, Value pair) {
    // First, find the pair
    for (const Card& card : hand.hand) {
        if (card.getValue() == pair) hand.bestFiveCards.emplace_back(card);
    }

    // Then, add the high card kickers
    int numKickers = 0;
    for (const Card& card : hand.hand) {
        if (card.getValue() != pair) {
            hand.bestFiveCards.emplace_back(card);
            numKickers++;
            if (numKickers == 3) break;
        }
    }
}

void findHighCard(PokerHand& hand) {
    // Add the first 5 high cards from the hand
    int numCards = 0;
    for (const Card& card : hand.hand) {
        hand.bestFiveCards.emplace_back(card);
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
    return hand >> NUM_VALUES * suit;
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
    uint64_t valueMask = 1ULL << value;
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