#include <gtest/gtest.h>
#include "../include/Card.h"
#include "../include/Player.h"
#include "../include/HandEvaluator.h"

class HandEvalTest : public ::testing::Test {
protected:
    HandEvaluator handEvaluator;
    shared_ptr<Player> player1;
    shared_ptr<Player> player2;
    shared_ptr<Player> player3;
    vector<Card> deck;

    HandEvalTest() {
        player1 = make_shared<Player>("P1", Position::SMALL_BLIND, 1000);
        player2 = make_shared<Player>("P2", Position::BIG_BLIND, 2000);
        player3 = make_shared<Player>("P3", Position::UTG, 3000);
        deck = createDeck();
    }

    void TearDown() override {
        handEvaluator.clearPlayerHands();
    }

    vector<Card> createDeck() {
        vector<Card> deck;
        for (int suit = static_cast<int>(Suit::HEARTS); suit <= static_cast<int>(Suit::SPADES); ++suit) {
            for (int val = static_cast<int>(Value::TWO); val <= static_cast<int>(Value::ACE); ++val) {
                deck.emplace_back(static_cast<Suit>(suit), static_cast<Value>(val));
            }
        }
        return deck;
    }

    vector<Card> selectCards(vector<Card>& deck, vector<pair<Suit, Value>>& cards) {
        vector<Card> selectedCards;
        for (const auto& pair : cards) {
            auto it = find_if(deck.begin(), deck.end(), [&](const Card& card) {
                return card.getSuit() == pair.first && card.getValue() == pair.second;
            });

            if (it != deck.end()) selectedCards.push_back(*it);
        }
        return selectedCards;
    }

    void dealCards(HandEvaluator& evaluator, shared_ptr<Player>& player, vector<Card>& cards) {
        for (const auto& card : cards) {
            handEvaluator.addDealtCard(player, card);
        }
    }

    const PokerHand& getPlayerHand(HandEvaluator& evaluator, shared_ptr<Player>& player) {
        const auto& playerHands = evaluator.getPlayerHandsMap();
        auto it = playerHands.find(player);
        const PokerHand& hand = it->second;
        return hand;
    }
};

TEST_F(HandEvalTest, RoyalFlush) {
    // DEAL CARDS
    vector<pair<Suit, Value>> criteria = {
        {Suit::HEARTS, Value::ACE},
        {Suit::DIAMONDS, Value::QUEEN},
        {Suit::SPADES, Value::ACE},
        {Suit::SPADES, Value::QUEEN},
        {Suit::SPADES, Value::JACK},
        {Suit::SPADES, Value::TEN},
        {Suit::SPADES, Value::KING},
    };
    vector<Card> cardsToDeal = selectCards(deck, criteria);
    dealCards(handEvaluator, player1, cardsToDeal);

    // EVALUATE HAND
    handEvaluator.evaluatePlayerHands();

    // FETCH HAND INFO
    PokerHand processedHand = getPlayerHand(handEvaluator, player1);

    // CONFIRM INFO IS ACCURATE
    EXPECT_EQ(processedHand.handSize, 7);
    EXPECT_EQ(processedHand.category, HandCategory::ROYAL_FLUSH);
    EXPECT_EQ(processedHand.bestFiveCards, vector<Card>({
        Card(Suit::SPADES, Value::ACE),
        Card(Suit::SPADES, Value::KING),
        Card(Suit::SPADES, Value::QUEEN),
        Card(Suit::SPADES, Value::JACK),
        Card(Suit::SPADES, Value::TEN)
    }));
};

TEST_F(HandEvalTest, StraightFlush) {
    // DEAL CARDS
    vector<pair<Suit, Value>> criteria = {
        {Suit::HEARTS, Value::ACE},
        {Suit::DIAMONDS, Value::QUEEN},
        {Suit::SPADES, Value::TWO},
        {Suit::SPADES, Value::ACE},
        {Suit::SPADES, Value::THREE},
        {Suit::SPADES, Value::FIVE},
        {Suit::SPADES, Value::FOUR},
    };
    vector<Card> cardsToDeal = selectCards(deck, criteria);
    dealCards(handEvaluator, player1, cardsToDeal);

    // EVALUATE HAND
    handEvaluator.evaluatePlayerHands();

    // FETCH HAND INFO
    PokerHand processedHand = getPlayerHand(handEvaluator, player1);

    // CONFIRM INFO IS ACCURATE
    EXPECT_EQ(processedHand.handSize, 7);
    EXPECT_EQ(processedHand.category, HandCategory::STRAIGHT_FLUSH);
    EXPECT_EQ(processedHand.bestFiveCards, vector<Card>({
        Card(Suit::SPADES, Value::FIVE),
        Card(Suit::SPADES, Value::FOUR),
        Card(Suit::SPADES, Value::THREE),
        Card(Suit::SPADES, Value::TWO),
        Card(Suit::SPADES, Value::ACE)
    }));
};

TEST_F(HandEvalTest, FourOfAKind) {
    // DEAL CARDS
    vector<pair<Suit, Value>> criteria = {
        {Suit::SPADES, Value::ACE},
        {Suit::DIAMONDS, Value::QUEEN},
        {Suit::SPADES, Value::QUEEN},
        {Suit::CLUBS, Value::QUEEN},
        {Suit::HEARTS, Value::QUEEN},
        {Suit::SPADES, Value::FIVE},
        {Suit::SPADES, Value::FOUR},
    };
    vector<Card> cardsToDeal = selectCards(deck, criteria);
    dealCards(handEvaluator, player1, cardsToDeal);

    // EVALUATE HAND
    handEvaluator.evaluatePlayerHands();

    // FETCH HAND INFO
    PokerHand processedHand = getPlayerHand(handEvaluator, player1);

    // CONFIRM INFO IS ACCURATE
    EXPECT_EQ(processedHand.handSize, 7);
    EXPECT_EQ(processedHand.category, HandCategory::FOUR_OF_A_KIND);
    EXPECT_EQ(processedHand.bestFiveCards, vector<Card>({
        Card(Suit::DIAMONDS, Value::QUEEN),
        Card(Suit::SPADES, Value::QUEEN),
        Card(Suit::CLUBS, Value::QUEEN),
        Card(Suit::HEARTS, Value::QUEEN),
        Card(Suit::SPADES, Value::ACE)
    }));
};

TEST_F(HandEvalTest, FullHouse) {
    // DEAL CARDS
    vector<pair<Suit, Value>> criteria = {
        {Suit::SPADES, Value::ACE},
        {Suit::DIAMONDS, Value::FIVE},
        {Suit::HEARTS, Value::ACE},
        {Suit::HEARTS, Value::TWO},
        {Suit::DIAMONDS, Value::ACE},
        {Suit::CLUBS, Value::FOUR},
        {Suit::SPADES, Value::FIVE}
    };
    vector<Card> cardsToDeal = selectCards(deck, criteria);
    dealCards(handEvaluator, player1, cardsToDeal);

    // EVALUATE HAND
    handEvaluator.evaluatePlayerHands();

    // FETCH HAND INFO
    PokerHand processedHand = getPlayerHand(handEvaluator, player1);

    // CONFIRM INFO IS ACCURATE
    EXPECT_EQ(processedHand.handSize, 7);
    EXPECT_EQ(processedHand.category, HandCategory::FULL_HOUSE);
    EXPECT_EQ(processedHand.bestFiveCards, vector<Card>({
        Card(Suit::SPADES, Value::ACE),
        Card(Suit::HEARTS, Value::ACE),
        Card(Suit::DIAMONDS, Value::ACE),
        Card(Suit::DIAMONDS, Value::FIVE),
        Card(Suit::SPADES, Value::FIVE)
    }));
};

TEST_F(HandEvalTest, Flush) {
    // DEAL CARDS
    vector<pair<Suit, Value>> criteria = {
        {Suit::SPADES, Value::TWO},
        {Suit::SPADES, Value::THREE},
        {Suit::SPADES, Value::FOUR},
        {Suit::SPADES, Value::FIVE},
        {Suit::DIAMONDS, Value::ACE},
        {Suit::HEARTS, Value::SIX},
        {Suit::SPADES, Value::SEVEN}
    };
    vector<Card> cardsToDeal = selectCards(deck, criteria);
    dealCards(handEvaluator, player1, cardsToDeal);

    // EVALUATE HAND
    handEvaluator.evaluatePlayerHands();

    // FETCH HAND INFO
    PokerHand processedHand = getPlayerHand(handEvaluator, player1);

    // CONFIRM INFO IS ACCURATE
    EXPECT_EQ(processedHand.handSize, 7);
    EXPECT_EQ(processedHand.category, HandCategory::FLUSH);
    EXPECT_EQ(processedHand.bestFiveCards, vector<Card>({
        Card(Suit::SPADES, Value::SEVEN),
        Card(Suit::SPADES, Value::FIVE),
        Card(Suit::SPADES, Value::FOUR),
        Card(Suit::SPADES, Value::THREE),
        Card(Suit::SPADES, Value::TWO)
    }));
};

TEST_F(HandEvalTest, Straight) {
    // DEAL CARDS
    vector<pair<Suit, Value>> criteria = {
        {Suit::SPADES, Value::TWO},
        {Suit::HEARTS, Value::THREE},
        {Suit::SPADES, Value::FOUR},
        {Suit::SPADES, Value::FIVE},
        {Suit::DIAMONDS, Value::ACE},
        {Suit::HEARTS, Value::SIX},
        {Suit::SPADES, Value::SEVEN}
    };
    vector<Card> cardsToDeal = selectCards(deck, criteria);
    dealCards(handEvaluator, player1, cardsToDeal);

    // EVALUATE HAND
    handEvaluator.evaluatePlayerHands();

    // FETCH HAND INFO
    PokerHand processedHand = getPlayerHand(handEvaluator, player1);

    // CONFIRM INFO IS ACCURATE
    EXPECT_EQ(processedHand.handSize, 7);
    EXPECT_EQ(processedHand.category, HandCategory::STRAIGHT);
    EXPECT_EQ(processedHand.bestFiveCards, vector<Card>({
        Card(Suit::SPADES, Value::SEVEN),
        Card(Suit::HEARTS, Value::SIX),
        Card(Suit::SPADES, Value::FIVE),
        Card(Suit::SPADES, Value::FOUR),
        Card(Suit::HEARTS, Value::THREE)
    }));
};

TEST_F(HandEvalTest, ThreeOfAKind) {
    // DEAL CARDS
    vector<pair<Suit, Value>> criteria = {
        {Suit::SPADES, Value::ACE},
        {Suit::HEARTS, Value::KING},
        {Suit::SPADES, Value::FOUR},
        {Suit::SPADES, Value::FIVE},
        {Suit::DIAMONDS, Value::ACE},
        {Suit::HEARTS, Value::ACE},
        {Suit::SPADES, Value::SEVEN}
    };
    vector<Card> cardsToDeal = selectCards(deck, criteria);
    dealCards(handEvaluator, player1, cardsToDeal);

    // EVALUATE HAND
    handEvaluator.evaluatePlayerHands();

    // FETCH HAND INFO
    PokerHand processedHand = getPlayerHand(handEvaluator, player1);

    // CONFIRM INFO IS ACCURATE
    EXPECT_EQ(processedHand.handSize, 7);
    EXPECT_EQ(processedHand.category, HandCategory::THREE_OF_A_KIND);
    EXPECT_EQ(processedHand.bestFiveCards, vector<Card>({
        Card(Suit::SPADES, Value::ACE),
        Card(Suit::DIAMONDS, Value::ACE),
        Card(Suit::HEARTS, Value::ACE),
        Card(Suit::HEARTS, Value::KING),
        Card(Suit::SPADES, Value::SEVEN)
    }));
};

TEST_F(HandEvalTest, TwoPair) {
    // DEAL CARDS
    vector<pair<Suit, Value>> criteria = {
        {Suit::SPADES, Value::ACE},
        {Suit::HEARTS, Value::KING},
        {Suit::SPADES, Value::QUEEN},
        {Suit::SPADES, Value::FIVE},
        {Suit::DIAMONDS, Value::ACE},
        {Suit::CLUBS, Value::KING},
        {Suit::SPADES, Value::SEVEN}
    };
    vector<Card> cardsToDeal = selectCards(deck, criteria);
    dealCards(handEvaluator, player1, cardsToDeal);

    // EVALUATE HAND
    handEvaluator.evaluatePlayerHands();

    // FETCH HAND INFO
    PokerHand processedHand = getPlayerHand(handEvaluator, player1);

    // CONFIRM INFO IS ACCURATE
    EXPECT_EQ(processedHand.handSize, 7);
    EXPECT_EQ(processedHand.category, HandCategory::TWO_PAIR);
    EXPECT_EQ(processedHand.bestFiveCards, vector<Card>({
        Card(Suit::SPADES, Value::ACE),
        Card(Suit::DIAMONDS, Value::ACE),
        Card(Suit::HEARTS, Value::KING),
        Card(Suit::CLUBS, Value::KING),
        Card(Suit::SPADES, Value::QUEEN)
    }));
};

TEST_F(HandEvalTest, OnePair) {
    // DEAL CARDS
    vector<pair<Suit, Value>> criteria = {
        {Suit::HEARTS, Value::EIGHT},
        {Suit::HEARTS, Value::KING},
        {Suit::SPADES, Value::QUEEN},
        {Suit::SPADES, Value::FIVE},
        {Suit::DIAMONDS, Value::EIGHT},
        {Suit::CLUBS, Value::JACK},
        {Suit::SPADES, Value::SEVEN}
    };
    vector<Card> cardsToDeal = selectCards(deck, criteria);
    dealCards(handEvaluator, player1, cardsToDeal);

    // EVALUATE HAND
    handEvaluator.evaluatePlayerHands();

    // FETCH HAND INFO
    PokerHand processedHand = getPlayerHand(handEvaluator, player1);

    // CONFIRM INFO IS ACCURATE
    EXPECT_EQ(processedHand.handSize, 7);
    EXPECT_EQ(processedHand.category, HandCategory::ONE_PAIR);
    EXPECT_EQ(processedHand.bestFiveCards, vector<Card>({
        Card(Suit::HEARTS, Value::EIGHT),
        Card(Suit::DIAMONDS, Value::EIGHT),
        Card(Suit::HEARTS, Value::KING),
        Card(Suit::SPADES, Value::QUEEN),
        Card(Suit::CLUBS, Value::JACK)
    }));
};

TEST_F(HandEvalTest, HighCard) {
    // DEAL CARDS
    vector<pair<Suit, Value>> criteria = {
        {Suit::HEARTS, Value::THREE},
        {Suit::DIAMONDS, Value::ACE},
        {Suit::CLUBS, Value::FOUR},
        {Suit::SPADES, Value::KING},
        {Suit::HEARTS, Value::SIX},
        {Suit::DIAMONDS, Value::JACK},
        {Suit::CLUBS, Value::EIGHT}
    };
    vector<Card> cardsToDeal = selectCards(deck, criteria);
    dealCards(handEvaluator, player1, cardsToDeal);

    // EVALUATE HAND
    handEvaluator.evaluatePlayerHands();

    // FETCH HAND INFO
    PokerHand processedHand = getPlayerHand(handEvaluator, player1);

    // CONFIRM INFO IS ACCURATE
    EXPECT_EQ(processedHand.handSize, 7);
    EXPECT_EQ(processedHand.category, HandCategory::HIGH_CARD);
    EXPECT_EQ(processedHand.bestFiveCards, vector<Card>({
        Card(Suit::DIAMONDS, Value::ACE),
        Card(Suit::SPADES, Value::KING),
        Card(Suit::DIAMONDS, Value::JACK),
        Card(Suit::CLUBS, Value::EIGHT),
        Card(Suit::HEARTS, Value::SIX)
    }));
};

TEST_F(HandEvalTest, TooFewCards) {
    // DEAL CARDS
    vector<pair<Suit, Value>> criteria = {
        {Suit::HEARTS, Value::THREE},
        {Suit::DIAMONDS, Value::ACE},
        {Suit::CLUBS, Value::FOUR},
        {Suit::SPADES, Value::KING},
    };
    vector<Card> cardsToDeal = selectCards(deck, criteria);
    dealCards(handEvaluator, player1, cardsToDeal);

    // EVALUATE HAND
    handEvaluator.evaluatePlayerHands();

    // FETCH HAND INFO
    PokerHand processedHand = getPlayerHand(handEvaluator, player1);

    // CONFIRM INFO IS ACCURATE
    EXPECT_EQ(processedHand.handSize, 4);
    EXPECT_EQ(processedHand.category, HandCategory::NONE);
};