#ifndef DEALER_H
#define DEALER_H

#include "Deck.h"
#include "Player.h"
#include "Board.h"
#include <vector>
using namespace std;

class Dealer {
private:
    Deck& deck;
    Board& board;
    explicit Dealer(Deck& deck, Board& board);
    Dealer(const Dealer&) = delete;             // Delete copy constructor 
    Dealer& operator=(const Dealer&) = delete;  // Delete copy assignment

public:
    static Dealer createDealer(Deck& deck, Board& board);
    void dealPlayer(shared_ptr<Player> player);
    void dealBoard(int numCards, bool burnCard = true);
    void resetDeck();

    // Utility Methods for Testing
    size_t getDealtCardCount() const;
    size_t getRemainingDeckSize() const;
    void printDeckState() const;
};

#endif // DEALER_H
