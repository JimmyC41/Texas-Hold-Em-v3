#ifndef HAND_EVALUATOR
#define HAND_EVALUATOR

#include "Player.h"
#include "Card.h"
#include <array>
#include <iostream>
#include <algorithm>
#include <assert.h>
using namespace std;

const int NUM_SUITS = 4;
const int MIN_HAND_SIZE = 5;
const int NUM_VALUES = 13;
const int MAX_HAND_SIZE = 7;

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
    uint64_t bitwise;       // 64 bit representation of hand
    HandCategory category;      // Category of hand (e.g. flush)
    vector<Card> hand;          // Sorted vector of hole and community cards  
    vector<Card> bestFiveCards; // Best 5 card combination
    int handSize;               // Sum of hole and community card count
};

class HandEvaluator {
private:
    // Hash map which keeps track of each player's hand (best 5 cards)
    unordered_map<shared_ptr<Player>, PokerHand> playerHands;

    // Evaluates category and bestFiveCards for a given hand
    void evaluateHand(PokerHand& hand);

    // Helper methods for hand evaluation
    // Sets hand category and calls helper methods below
    bool isRoyalFlush(PokerHand& hand);
    bool isStraightFlush(PokerHand& hand);
    bool isFullHouse(PokerHand& hand);
    bool isFlush(PokerHand& hand);
    bool isStraight(PokerHand& hand);
    bool isNOfAKind(PokerHand& hand, int n);
    bool isTwoPair(PokerHand& hand);
    bool isHighCard(PokerHand& hand);

    // Helper methods to find the best 5 card combination
    void findStraight(PokerHand& hand, Value highCard, bool isFlush, Suit flushSuit);
    void findFourOfAKind(PokerHand& hand, Value quads);
    void findFullHouse(PokerHand& hand, Value trips, Value pair);
    void findFlush(PokerHand& hand, Suit suit);
    void findThreeOfAKind(PokerHand& hand, Value trips);
    void findTwoPair(PokerHand& hand, Value pairOne, Value pairTwo);
    void findOnePair(PokerHand& hand, Value pair);
    void findHighCard(PokerHand& hand);

    // Helper methods for bitwise operations (used to categorise hands)
    uint64_t getAllSuitsMask(uint64_t hand);
    uint64_t getSuitMask(uint64_t hand, int suit);
    int countSetBits(uint64_t mask);
    int countBitsForValue(uint64_t hand, int value);
    Value straightMaskToHighCard(uint64_t straightMask);

public:
    // Updates each player's PokerHand in the hashmap
    void addDealtCard(shared_ptr<Player> player, const Card& card);

    // Evaluates the hand category and best 5 card combination for each player in the hashmap
    void evaluatePlayerHands();

    // Returns a vector of players sorted by the strength of their hand.
    // Called when action is finished and pots must be awarded.
    // Parsed as an argument to the awardPots method in PotManager.
    vector<shared_ptr<Player>> getPlayerHandRanking();

    // Clears the playerHands hash map. Called at the end of each round.
    void resetPlayerHands();
};

#endif // HAND_EVALUATOR