#ifndef BOARD_H
#define BOARD_H

#include "Card.h"
#include <vector>
#include <stdexcept>
#include <string>
using namespace std;

class Board {
private:
    vector<Card> communityCards;
    void validateAddition() const;
public:
    Board();
    void addCommunityCard(const Card& card);
    void resetBoard();
    void printBoardState() const;
    bool isFlopDealt() const;
    bool isTurnDealt() const;
    bool isRiverDealt() const;
    const vector<Card>& getCommunityCards() const;
    size_t getCommunityCardCount() const;
};

#endif // BOARD_H
