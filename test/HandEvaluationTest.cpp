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
    shared_ptr<Player> player4;
    shared_ptr<Player> player5;
    shared_ptr<Player> player6;
    shared_ptr<Player> player7;
    shared_ptr<Player> player8;
    shared_ptr<Player> player9;
    vector<Card> deck;

    HandEvalTest() {
        player1 = make_shared<Player>("P1", Position::SMALL_BLIND, 1000);
        player2 = make_shared<Player>("P2", Position::BIG_BLIND, 1000);
        player3 = make_shared<Player>("P3", Position::UTG, 1000);
        player4 = make_shared<Player>("P4", Position::UTG_1, 1000);
        player5 = make_shared<Player>("P5", Position::MIDDLE, 1000);
        player6 = make_shared<Player>("P6", Position::LOJACK, 1000);
        player7 = make_shared<Player>("P7", Position::HIJACK, 1000);
        player8 = make_shared<Player>("P8", Position::CUT_OFF, 1000);
        player9 = make_shared<Player>("P9", Position::DEALER, 1000);
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

    bool isPlayerVectorsEqual(const vector<shared_ptr<Player>>& vec1, const vector<shared_ptr<Player>>& vec2) {
        if (vec1.size() != vec2.size()) return false;
        for (size_t i = 0; i < vec1.size(); ++i) {
            if (vec1[i] != vec2[i]) return false;
        }
        return true;
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

TEST_F(HandEvalTest, FullHouseTie) {
    // DEAL CARDS
    vector<pair<Suit, Value>> AcesFullOfQueens = {
        {Suit::HEARTS, Value::ACE},
        {Suit::DIAMONDS, Value::ACE},
        {Suit::SPADES, Value::QUEEN},
        {Suit::DIAMONDS, Value::THREE},
        {Suit::HEARTS, Value::THREE},
        {Suit::SPADES, Value::ACE},
        {Suit::CLUBS, Value::QUEEN}
    };
    vector<Card> cardsToDealPlayer1 = selectCards(deck, AcesFullOfQueens);
    dealCards(handEvaluator, player1, cardsToDealPlayer1);

    vector<pair<Suit, Value>> QueensFullOfAces = {
        {Suit::HEARTS, Value::ACE},
        {Suit::DIAMONDS, Value::ACE},
        {Suit::SPADES, Value::QUEEN},
        {Suit::DIAMONDS, Value::THREE},
        {Suit::HEARTS, Value::THREE},
        {Suit::DIAMONDS, Value::QUEEN},
        {Suit::HEARTS, Value::QUEEN}
    };
    vector<Card> cardsToDealPlayer2 = selectCards(deck, QueensFullOfAces);
    dealCards(handEvaluator, player2, cardsToDealPlayer2);

    // EVALUATE HANDS
    handEvaluator.evaluatePlayerHands();

    // SORT PLAYERS BY HAND AND CHECK IF EXPECTED
    vector<shared_ptr<Player>> sortedPlayers = handEvaluator.sortPlayersByHandStrength();
    vector<shared_ptr<Player>> expectedPlayers = {player1, player2};
    EXPECT_TRUE(isPlayerVectorsEqual(sortedPlayers, expectedPlayers));
};

TEST_F(HandEvalTest, FlushTie) {
    // DEAL CARDS
    vector<pair<Suit, Value>> AceHighHearts = {
        {Suit::DIAMONDS, Value::TWO},
        {Suit::DIAMONDS, Value::THREE},
        {Suit::HEARTS, Value::FOUR},
        {Suit::HEARTS, Value::FIVE},
        {Suit::HEARTS, Value::SIX},
        {Suit::HEARTS, Value::ACE},
        {Suit::HEARTS, Value::KING}
    };
    vector<Card> cardsToDealPlayer1 = selectCards(deck, AceHighHearts);
    dealCards(handEvaluator, player1, cardsToDealPlayer1);

    vector<pair<Suit, Value>> QueenHighHearts = {
        {Suit::DIAMONDS, Value::TWO},
        {Suit::DIAMONDS, Value::THREE},
        {Suit::HEARTS, Value::FOUR},
        {Suit::HEARTS, Value::FIVE},
        {Suit::HEARTS, Value::SIX},
        {Suit::HEARTS, Value::QUEEN},
        {Suit::HEARTS, Value::JACK}
    };
    vector<Card> cardsToDealPlayer2 = selectCards(deck, QueenHighHearts);
    dealCards(handEvaluator, player2, cardsToDealPlayer2);

    vector<pair<Suit, Value>> EightHighStraightFlush = {
        {Suit::DIAMONDS, Value::TWO},
        {Suit::DIAMONDS, Value::THREE},
        {Suit::HEARTS, Value::FOUR},
        {Suit::HEARTS, Value::FIVE},
        {Suit::HEARTS, Value::SIX},
        {Suit::HEARTS, Value::SEVEN},
        {Suit::HEARTS, Value::EIGHT}
    };
    vector<Card> cardsToDealPlayer3 = selectCards(deck, EightHighStraightFlush);
    dealCards(handEvaluator, player3, cardsToDealPlayer3);

    vector<pair<Suit, Value>> TenHighHearts = {
        {Suit::DIAMONDS, Value::TWO},
        {Suit::DIAMONDS, Value::THREE},
        {Suit::HEARTS, Value::FOUR},
        {Suit::HEARTS, Value::FIVE},
        {Suit::HEARTS, Value::SIX},
        {Suit::HEARTS, Value::NINE},
        {Suit::HEARTS, Value::TEN}
    };
    vector<Card> cardsToDealPlayer4 = selectCards(deck, TenHighHearts);
    dealCards(handEvaluator, player4, cardsToDealPlayer4);

    // EVALUATE HANDS
    handEvaluator.evaluatePlayerHands();

    // SORT PLAYERS BY HAND AND CHECK IF EXPECTED
    vector<shared_ptr<Player>> sortedPlayers = handEvaluator.sortPlayersByHandStrength();
    vector<shared_ptr<Player>> expectedPlayers = {player3, player1, player2, player4};
    EXPECT_TRUE(isPlayerVectorsEqual(sortedPlayers, expectedPlayers));
};

TEST_F(HandEvalTest, StraightTie) {
    // DEAL CARDS
    vector<pair<Suit, Value>> NineHighStraight = {
        {Suit::DIAMONDS, Value::ACE},
        {Suit::DIAMONDS, Value::KING},
        {Suit::HEARTS, Value::FIVE},
        {Suit::HEARTS, Value::SIX},
        {Suit::HEARTS, Value::SEVEN},
        {Suit::SPADES, Value::EIGHT},
        {Suit::SPADES, Value::NINE}
    };
    vector<Card> cardsToDealPlayer1 = selectCards(deck, NineHighStraight);
    dealCards(handEvaluator, player1, cardsToDealPlayer1);

    vector<pair<Suit, Value>> EightHighStraight = {
        {Suit::DIAMONDS, Value::ACE},
        {Suit::DIAMONDS, Value::KING},
        {Suit::HEARTS, Value::FIVE},
        {Suit::HEARTS, Value::SIX},
        {Suit::HEARTS, Value::SEVEN},
        {Suit::CLUBS, Value::FOUR},
        {Suit::CLUBS, Value::EIGHT}
    };
    vector<Card> cardsToDealPlayer2 = selectCards(deck, EightHighStraight);
    dealCards(handEvaluator, player2, cardsToDealPlayer2);

    vector<pair<Suit, Value>> NineHighStraightFlush = {
        {Suit::DIAMONDS, Value::ACE},
        {Suit::DIAMONDS, Value::KING},
        {Suit::HEARTS, Value::FIVE},
        {Suit::HEARTS, Value::SIX},
        {Suit::HEARTS, Value::SEVEN},
        {Suit::HEARTS, Value::EIGHT},
        {Suit::HEARTS, Value::NINE}
    };
    vector<Card> cardsToDealPlayer3 = selectCards(deck, NineHighStraightFlush);
    dealCards(handEvaluator, player3, cardsToDealPlayer3);

    // EVALUATE HANDS
    handEvaluator.evaluatePlayerHands();

    // SORT PLAYERS BY HAND AND CHECK IF EXPECTED
    vector<shared_ptr<Player>> sortedPlayers = handEvaluator.sortPlayersByHandStrength();
    vector<shared_ptr<Player>> expectedPlayers = {player3, player1, player2};
    EXPECT_TRUE(isPlayerVectorsEqual(sortedPlayers, expectedPlayers));
};

TEST_F(HandEvalTest, ThreeOfAKindTie) {
    // DEAL CARDS
    vector<pair<Suit, Value>> SixTripsQueenKicker = {
        {Suit::CLUBS, Value::THREE},
        {Suit::SPADES, Value::THREE},
        {Suit::HEARTS, Value::SIX},
        {Suit::DIAMONDS, Value::SIX},
        {Suit::CLUBS, Value::TWO},
        {Suit::SPADES, Value::SIX},
        {Suit::HEARTS, Value::QUEEN}
    };
    vector<Card> cardsToDealPlayer1 = selectCards(deck, SixTripsQueenKicker);
    dealCards(handEvaluator, player1, cardsToDealPlayer1);

    vector<pair<Suit, Value>> SixTripsKingKicker = {
        {Suit::CLUBS, Value::THREE},
        {Suit::SPADES, Value::THREE},
        {Suit::HEARTS, Value::SIX},
        {Suit::DIAMONDS, Value::SIX},
        {Suit::CLUBS, Value::TWO},
        {Suit::CLUBS, Value::SIX},
        {Suit::HEARTS, Value::KING}
    };
    vector<Card> cardsToDealPlayer2 = selectCards(deck, SixTripsKingKicker);
    dealCards(handEvaluator, player2, cardsToDealPlayer2);

    // EVALUATE HANDS
    handEvaluator.evaluatePlayerHands();

    // SORT PLAYERS BY HAND AND CHECK IF EXPECTED
    vector<shared_ptr<Player>> sortedPlayers = handEvaluator.sortPlayersByHandStrength();
    vector<shared_ptr<Player>> expectedPlayers = {player2, player1};
    EXPECT_TRUE(isPlayerVectorsEqual(sortedPlayers, expectedPlayers));
};

TEST_F(HandEvalTest, TwoPairTie) {
    // DEAL CARDS
    vector<pair<Suit, Value>> KingKicker = {
        {Suit::CLUBS, Value::ACE},
        {Suit::SPADES, Value::ACE},
        {Suit::CLUBS, Value::TWO},
        {Suit::CLUBS, Value::THREE},
        {Suit::CLUBS, Value::FOUR},
        {Suit::DIAMONDS, Value::FOUR},
        {Suit::DIAMONDS, Value::KING}
    };
    vector<Card> cardsToDealPlayer1 = selectCards(deck, KingKicker);
    dealCards(handEvaluator, player1, cardsToDealPlayer1);

    vector<pair<Suit, Value>> QueenKicker = {
        {Suit::CLUBS, Value::ACE},
        {Suit::SPADES, Value::ACE},
        {Suit::CLUBS, Value::TWO},
        {Suit::CLUBS, Value::THREE},
        {Suit::CLUBS, Value::FOUR},
        {Suit::HEARTS, Value::FOUR},
        {Suit::DIAMONDS, Value::QUEEN}
    };
    vector<Card> cardsToDealPlayer2 = selectCards(deck, QueenKicker);
    dealCards(handEvaluator, player2, cardsToDealPlayer2);

    vector<pair<Suit, Value>> JackKicker = {
        {Suit::CLUBS, Value::ACE},
        {Suit::SPADES, Value::ACE},
        {Suit::CLUBS, Value::TWO},
        {Suit::CLUBS, Value::THREE},
        {Suit::CLUBS, Value::FOUR},
        {Suit::SPADES, Value::FOUR},
        {Suit::DIAMONDS, Value::JACK}
    };
    vector<Card> cardsToDealPlayer3 = selectCards(deck, JackKicker);
    dealCards(handEvaluator, player3, cardsToDealPlayer3);

    // EVALUATE HANDS
    handEvaluator.evaluatePlayerHands();

    // SORT PLAYERS BY HAND AND CHECK IF EXPECTED
    vector<shared_ptr<Player>> sortedPlayers = handEvaluator.sortPlayersByHandStrength();
    vector<shared_ptr<Player>> expectedPlayers = {player1, player2, player3};
    EXPECT_TRUE(isPlayerVectorsEqual(sortedPlayers, expectedPlayers));
};

TEST_F(HandEvalTest, OnePairTie) {
    // DEAL CARDS
    vector<pair<Suit, Value>> KingKicker = {
        {Suit::CLUBS, Value::TWO},
        {Suit::DIAMONDS, Value::FOUR},
        {Suit::SPADES, Value::FIVE},
        {Suit::HEARTS, Value::SIX},
        {Suit::CLUBS, Value::SEVEN},
        {Suit::DIAMONDS, Value::KING},
        {Suit::SPADES, Value::SEVEN}
    };
    vector<Card> cardsToDealPlayer1 = selectCards(deck, KingKicker);
    dealCards(handEvaluator, player1, cardsToDealPlayer1);

    vector<pair<Suit, Value>> AceKicker = {
        {Suit::CLUBS, Value::TWO},
        {Suit::DIAMONDS, Value::FOUR},
        {Suit::SPADES, Value::FIVE},
        {Suit::HEARTS, Value::SIX},
        {Suit::CLUBS, Value::SEVEN},
        {Suit::DIAMONDS, Value::ACE},
        {Suit::HEARTS, Value::SEVEN}
    };
    vector<Card> cardsToDealPlayer2 = selectCards(deck, AceKicker);
    dealCards(handEvaluator, player2, cardsToDealPlayer2);

    vector<pair<Suit, Value>> JackKicker = {
        {Suit::CLUBS, Value::TWO},
        {Suit::DIAMONDS, Value::FOUR},
        {Suit::SPADES, Value::FIVE},
        {Suit::HEARTS, Value::SIX},
        {Suit::CLUBS, Value::SEVEN},
        {Suit::DIAMONDS, Value::JACK},
        {Suit::DIAMONDS, Value::QUEEN}
    };
    vector<Card> cardsToDealPlayer3 = selectCards(deck, JackKicker);
    dealCards(handEvaluator, player3, cardsToDealPlayer3);

    // EVALUATE HANDS
    handEvaluator.evaluatePlayerHands();

    // SORT PLAYERS BY HAND AND CHECK IF EXPECTED
    vector<shared_ptr<Player>> sortedPlayers = handEvaluator.sortPlayersByHandStrength();
    vector<shared_ptr<Player>> expectedPlayers = {player2, player1, player3};
    EXPECT_TRUE(isPlayerVectorsEqual(sortedPlayers, expectedPlayers));
};

TEST_F(HandEvalTest, ComplexHandRankings) {
    vector<pair<Suit, Value>> StraightFlush = {
        {Suit::CLUBS, Value::FOUR},
        {Suit::CLUBS, Value::FIVE},
        {Suit::CLUBS, Value::SIX},
        {Suit::SPADES, Value::SEVEN},
        {Suit::SPADES, Value::ACE},
        {Suit::CLUBS, Value::TWO},
        {Suit::CLUBS, Value::THREE}
    };
    vector<Card> cardsToDealPlayer1 = selectCards(deck, StraightFlush);
    dealCards(handEvaluator, player1, cardsToDealPlayer1);

    vector<pair<Suit, Value>> KingHighFlush = {
        {Suit::CLUBS, Value::FOUR},
        {Suit::CLUBS, Value::FIVE},
        {Suit::CLUBS, Value::SIX},
        {Suit::SPADES, Value::SEVEN},
        {Suit::SPADES, Value::ACE},
        {Suit::CLUBS, Value::KING},
        {Suit::CLUBS, Value::QUEEN}
    };
    vector<Card> cardsToDealPlayer2 = selectCards(deck, StraightFlush);
    dealCards(handEvaluator, player2, cardsToDealPlayer2);

    vector<pair<Suit, Value>> FourFiveTwoPair = {
        {Suit::CLUBS, Value::FOUR},
        {Suit::CLUBS, Value::FIVE},
        {Suit::CLUBS, Value::SIX},
        {Suit::SPADES, Value::SEVEN},
        {Suit::SPADES, Value::ACE},
        {Suit::HEARTS, Value::FOUR},
        {Suit::HEARTS, Value::FIVE}
    };
    vector<Card> cardsToDealPlayer3 = selectCards(deck, FourFiveTwoPair);
    dealCards(handEvaluator, player3, cardsToDealPlayer3);

    vector<pair<Suit, Value>> NineHighStraight = {
        {Suit::CLUBS, Value::FOUR},
        {Suit::CLUBS, Value::FIVE},
        {Suit::CLUBS, Value::SIX},
        {Suit::SPADES, Value::SEVEN},
        {Suit::SPADES, Value::ACE},
        {Suit::HEARTS, Value::EIGHT},
        {Suit::HEARTS, Value::NINE}
    };
    vector<Card> cardsToDealPlayer4 = selectCards(deck, NineHighStraight);
    dealCards(handEvaluator, player4, cardsToDealPlayer4);

    vector<pair<Suit, Value>> AcePairTwoEightKicker = {
        {Suit::CLUBS, Value::FOUR},
        {Suit::CLUBS, Value::FIVE},
        {Suit::CLUBS, Value::SIX},
        {Suit::SPADES, Value::SEVEN},
        {Suit::SPADES, Value::ACE},
        {Suit::HEARTS, Value::ACE},
        {Suit::HEARTS, Value::EIGHT}
    };
    vector<Card> cardsToDealPlayer5 = selectCards(deck, AcePairTwoEightKicker);
    dealCards(handEvaluator, player5, cardsToDealPlayer5);

    vector<pair<Suit, Value>> AcePairTwoTenKicker = {
        {Suit::CLUBS, Value::FOUR},
        {Suit::CLUBS, Value::FIVE},
        {Suit::CLUBS, Value::SIX},
        {Suit::SPADES, Value::SEVEN},
        {Suit::SPADES, Value::ACE},
        {Suit::DIAMONDS, Value::ACE},
        {Suit::HEARTS, Value::TEN}
    };
    vector<Card> cardsToDealPlayer6 = selectCards(deck, AcePairTwoTenKicker);
    dealCards(handEvaluator, player6, cardsToDealPlayer6);

    vector<pair<Suit, Value>> AceSevenTwoPair = {
        {Suit::CLUBS, Value::FOUR},
        {Suit::CLUBS, Value::FIVE},
        {Suit::CLUBS, Value::SIX},
        {Suit::SPADES, Value::SEVEN},
        {Suit::SPADES, Value::ACE},
        {Suit::CLUBS, Value::ACE},
        {Suit::HEARTS, Value::SEVEN}
    };
    vector<Card> cardsToDealPlayer7 = selectCards(deck, AceSevenTwoPair);
    dealCards(handEvaluator, player7, cardsToDealPlayer7);

    vector<pair<Suit, Value>> JackHighFlush = {
        {Suit::CLUBS, Value::FOUR},
        {Suit::CLUBS, Value::FIVE},
        {Suit::CLUBS, Value::SIX},
        {Suit::SPADES, Value::SEVEN},
        {Suit::SPADES, Value::ACE},
        {Suit::CLUBS, Value::JACK},
        {Suit::CLUBS, Value::TEN}
    };
    vector<Card> cardsToDealPlayer8 = selectCards(deck, JackHighFlush);
    dealCards(handEvaluator, player8, cardsToDealPlayer8);

    vector<pair<Suit, Value>> TripFours = {
        {Suit::CLUBS, Value::FOUR},
        {Suit::CLUBS, Value::FIVE},
        {Suit::CLUBS, Value::SIX},
        {Suit::SPADES, Value::SEVEN},
        {Suit::SPADES, Value::ACE},
        {Suit::DIAMONDS, Value::FOUR},
        {Suit::SPADES, Value::FOUR}
    };
    vector<Card> cardsToDealPlayer9 = selectCards(deck, TripFours);
    dealCards(handEvaluator, player9, cardsToDealPlayer9);

    // EVALUATE HANDS
    handEvaluator.evaluatePlayerHands();

    // SORT PLAYERS BY HAND AND CHECK IF EXPECTED
    vector<shared_ptr<Player>> sortedPlayers = handEvaluator.sortPlayersByHandStrength();
    vector<shared_ptr<Player>> expectedPlayers = {player1, player2, player8, player4, player9, player7, player4, player6, player5};
    EXPECT_TRUE(isPlayerVectorsEqual(sortedPlayers, expectedPlayers));
}