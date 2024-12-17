#ifndef HAND_EVALUATOR
#define HAND_EVALUATOR

#include "Player.h"
#include "Card.h"
#include <array>
#include <iostream>
using namespace std;

const int MAX_HAND_SIZE = 7;
const int NUM_SUITS = 4;
const int SUIT_SIZE = 13;

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

typedef struct PokerHand {
    uint64_t handBitMask;
    HandCategory category;
    vector<Card> bestHand;
};

class HandEvaluator {
private:
    // Hash map which keeps track of each player's hand (best 5 cards)
    unordered_map<shared_ptr<Player>&, PokerHand> playerHands;

    // Updates the HandCategory of a player. Called after a card is added.
    void getHandCategory();

    // Updates the bestHand of a player. Called after updating the HandCategory.
    vector<Card> getBestHand(HandCategory category);
public:
    // Updates each player's PokerHand in the hashmap.
    // winningHand and handBitMask is iteratively updated as cards are added.
    void addDealtCard(shared_ptr<Player> player, const Card& card);

    // Returns a vector of players sorted by the strength of their hand.
    // Called when action is finished and pots must be awarded.
    // Parsed as an argument to the awardPots method in PotManager.
    vector<shared_ptr<Player>> getPlayerHandRanking();

    // Clears the playerHands hash map. Called at the end of each round.
    void resetPlayerHands();
};

#endif // HAND_EVALUATOR