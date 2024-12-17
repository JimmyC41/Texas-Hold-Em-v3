#include "../include/HandEvaluator.h"

// PokerHand Struct

PokerHand::PokerHand() : handBitMask(0), handSize(0), category(HandCategory::NONE), bestHand() {}


// HandEvaluator Class

HandEvaluator::HandEvaluator() : playerHands() {}

void HandEvaluator::addDealtCard(shared_ptr<Player> player, const Card& card) {
    PokerHand& hand = playerHands[player];
    
}

// Helper functions for hand categorisation

HandCategory HandEvaluator::computeHandCategory(PokerHand& hand) {
    if (hand.handSize < MIN_HAND_SIZE) return NONE;
    if (isRoyalFlush(hand.handBitMask)) return ROYAL_FLUSH;
    if (isStraightFlush(hand.handBitMask)) return STRAIGHT_FLUSH;
    if (isQuads(hand.handBitMask)) return FOUR_OF_A_KIND;
    if (isFullHouse(hand.handBitMask)) return FULL_HOUSE;
    if (isFlush(hand.handBitMask)) return FLUSH;
    if (isStraight(hand.handBitMask)) return STRAIGHT;
    if (isTrips(hand.handSize)) return THREE_OF_A_KIND;
    if (isTwoPair(hand.handBitMask)) return TWO_PAIR;
    if (isOnePair(hand.handBitMask)) return ONE_PAIR;
    return HIGH_CARD;
}

bool HandEvaluator::isRoyalFlush(uint64_t hand) {
    for (int suit = static_cast<int>(Suit::HEARTS); suit <= static_cast<int>(Suit::SPADES); ++suit) {
        uint64_t suitMask = getSuitMask(hand, suit);
        if ((suitMask && BITMASK_ACE_STRAIGHT) == BITMASK_ACE_STRAIGHT) return true;
    }
    return false;
}

bool HandEvaluator::isStraightFlush(uint64_t hand) {
    for (int suit = static_cast<int>(Suit::HEARTS); suit <= static_cast<int>(Suit::SPADES); ++suit) {
        uint64_t suitMask = getSuitMask(hand, suit);
        for (const uint64_t mask : straightMasks) {
            if ((suitMask & mask) == mask) return true;
        }
    }
    return false;
}

bool HandEvaluator::isQuads(uint64_t hand) {

}

bool HandEvaluator::isFullHouse(uint64_t hand) {

}

bool HandEvaluator::isFlush(uint64_t hand) {
    for (int suit = static_cast<int>(Suit::HEARTS); suit <= static_cast<int>(Suit::SPADES); ++suit) {
        uint64_t suitMask = getSuitMask(hand, suit);
        if (countSetBits(suitMask) >= MIN_HAND_SIZE) return true;
    }
    return false;
}

bool HandEvaluator::isStraight(uint64_t hand) {
    uint64_t handNoSuits = getAllSuitsMask(hand);
    for (const uint64_t mask : straightMasks) {
        if ((handNoSuits & mask) == mask) return true;
    }
    return false;
}

bool HandEvaluator::isTrips(uint64_t hand) {

}

bool HandEvaluator::isTwoPair(uint64_t hand) {

}

bool HandEvaluator::isOnePair(uint64_t hand) {

}

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