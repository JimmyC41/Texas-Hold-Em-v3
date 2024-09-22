#include <gtest/gtest.h>
#include "../include/Dealer.h"
#include "../include/Deck.h"
#include "../include/Board.h"
#include "../include/Player.h"
#include <memory>  // Include for make_shared

class DealTest : public ::testing::Test {
protected:
    Deck deck;
    Board board;
    Dealer dealer;
    shared_ptr<Player> player1;
    shared_ptr<Player> player2;
    shared_ptr<Player> player3;
    shared_ptr<Player> player4;

    DealTest()
        : dealer(Dealer::createDealer(deck, board)),
          player1(make_shared<Player>("P1", Position::SMALL_BLIND, 500)),
          player2(make_shared<Player>("P2", Position::BIG_BLIND, 500)),
          player3(make_shared<Player>("P3", Position::UTG, 500)),
          player4(make_shared<Player>("P4", Position::UTG_1, 500)) {}

    void TearDown() override {
        dealer.resetDeck();
        board.resetBoard();
        player1->resetHand();
        player2->resetHand();
        player3->resetHand();
        player4->resetHand();
    }
};

bool allUniqueCards(const std::vector<Card>& cards) {
    std::set<Card> uniqueCards(cards.begin(), cards.end());
    std::cout << "Set size: " << uniqueCards.size() << " and dealt cards size: " << cards.size() << std::endl;
    return uniqueCards.size() == cards.size();
}

TEST_F(DealTest, DealToPlayers) {
    dealer.dealPlayer(player1);
    dealer.dealPlayer(player2);
    dealer.dealPlayer(player3);
    dealer.dealPlayer(player4);
    dealer.dealPlayer(player1);
    dealer.dealPlayer(player2);
    dealer.dealPlayer(player3);
    dealer.dealPlayer(player4);

    EXPECT_EQ(player1->getHand().size(), 2);
    EXPECT_EQ(player2->getHand().size(), 2);
    EXPECT_EQ(player3->getHand().size(), 2);
    EXPECT_EQ(player4->getHand().size(), 2);
    EXPECT_EQ(dealer.getRemainingDeckSize(), DECK_SIZE - 8);
}

TEST_F(DealTest, DealToBoard) {
    dealer.dealBoard(3, true);

    EXPECT_EQ(board.getCommunityCardCount(), 3);
    EXPECT_EQ(dealer.getRemainingDeckSize(), DECK_SIZE - 4);

    dealer.dealBoard(1, true);

    EXPECT_EQ(board.getCommunityCardCount(), 4);
    EXPECT_EQ(dealer.getRemainingDeckSize(), DECK_SIZE - 6);

    dealer.dealBoard(1, true);

    EXPECT_EQ(board.getCommunityCardCount(), 5);
    EXPECT_EQ(dealer.getRemainingDeckSize(), DECK_SIZE - 8);
}

TEST_F(DealTest, UniqueCardsDealt) {
    dealer.dealPlayer(player1);
    dealer.dealPlayer(player2);
    dealer.dealPlayer(player3);
    dealer.dealPlayer(player4);

    dealer.dealBoard(3, true);
    dealer.dealBoard(1, true);
    dealer.dealBoard(1, true);

    std::vector<Card> allDealtCards;
    allDealtCards.insert(allDealtCards.end(), player1->getHand().begin(), player1->getHand().end());
    allDealtCards.insert(allDealtCards.end(), player2->getHand().begin(), player2->getHand().end());
    allDealtCards.insert(allDealtCards.end(), player3->getHand().begin(), player3->getHand().end());
    allDealtCards.insert(allDealtCards.end(), player4->getHand().begin(), player4->getHand().end());
    allDealtCards.insert(allDealtCards.end(), board.getCommunityCards().begin(), board.getCommunityCards().end());

    EXPECT_TRUE(allUniqueCards(allDealtCards)) << "Not all dealt cards are unique!";
}

TEST_F(DealTest, ResetDeck) {
    dealer.dealPlayer(player1);
    dealer.dealPlayer(player3);
    dealer.dealBoard(3, true);

    size_t cardsBeforeReset = dealer.getRemainingDeckSize();
    dealer.resetDeck();

    EXPECT_EQ(dealer.getRemainingDeckSize(), DECK_SIZE);
    EXPECT_NE(dealer.getRemainingDeckSize(), cardsBeforeReset);
}

TEST_F(DealTest, ResetBoard) {
    dealer.dealBoard(3, true);
    board.resetBoard();
    EXPECT_EQ(board.getCommunityCardCount(), 0);

    dealer.dealBoard(2, true);
    board.resetBoard();
    EXPECT_EQ(board.getCommunityCardCount(), 0);

    dealer.dealBoard(1, true);
    board.resetBoard();
    EXPECT_EQ(board.getCommunityCardCount(), 0);
}

TEST_F(DealTest, PlayerChipsModification) {
    player1->addChips(100); // 500 + 100
    player1->reduceChips(300); // 600 - 300
    player1->addChips(800); // 300 + 800
    player1->reduceChips(900); // 1100 - 900
    player1->reduceChips(100); // 200 - 100
    player1->addChips(1000); // 100 + 1000

    EXPECT_EQ(player1->getChips(), 1100);

    player2->addChips(1200);
    player2->reduceChips(600);
    EXPECT_EQ(player2->getChips(), 1100);

    player3->reduceChips(500);
    EXPECT_EQ(player3->getChips(), 0);

    player4->reduceChips(500);
    player4->addChips(500);
    EXPECT_EQ(player4->getChips(), 500);
}
