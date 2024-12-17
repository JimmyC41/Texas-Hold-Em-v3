#ifndef HAND_EVALUATOR
#define HAND_EVALUATOR

#include "Player.h"
#include "Card.h"
#include <array>
#include <iostream>
using namespace std;

const int MIN_HAND_SIZE = 5;
const int NUM_VALUES = 13;

enum HandCategory {
    NONE, // In Preflop, hand size is less than 5
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
};

const uint64_t straightMasks[] = {
    0x100F, // 5-high
    0x1F, // 6-high
    0x3E, // 7-high
    0x7C, // 8-high
    0xF8, // 9-high
    0x1F0, // 10-high
    0x3E0, // J-high
    0x7C0, // Q-high
    0xF80, // K-high
    0x1F00 // A-high
};

const uint64_t BITMASK_13_BITS = 0x1FFF;
const uint64_t BITMASK_ACE_STRAIGHT = 0x1F00;

typedef struct PokerHand {
    uint64_t handBitMask;
    HandCategory category;
    vector<Card> bestHand;
    int handSize;
};

class HandEvaluator {
private:
    // Hash map which keeps track of each player's hand (best 5 cards)
    unordered_map<shared_ptr<Player>, PokerHand> playerHands;

    // Helper methods for hand categorisation
    HandCategory computeHandCategory(PokerHand& hand);
    bool isRoyalFlush(uint64_t hand);
    bool isStraightFlush(uint64_t hand);
    bool isQuads(uint64_t hand);
    bool isFullHouse(uint64_t hand);
    bool isFlush(uint64_t hand);
    bool isStraight(uint64_t hand);
    bool isTrips(uint64_t hand);
    bool isTwoPair(uint64_t hand);
    bool isOnePair(uint64_t hand);
    uint64_t getAllSuitsMask(uint64_t hand);
    uint64_t getSuitMask(uint64_t hand, int suit);
    int countSetBits(uint64_t mask);

public:
    // Updates each player's PokerHand in the hashmap.
    void addDealtCard(shared_ptr<Player> player, const Card& card);

    // Returns a vector of players sorted by the strength of their hand.
    // Called when action is finished and pots must be awarded.
    // Parsed as an argument to the awardPots method in PotManager.
    vector<shared_ptr<Player>> getPlayerHandRanking();

    // Clears the playerHands hash map. Called at the end of each round.
    void resetPlayerHands();
};

#endif // HAND_EVALUATOR