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
    void clearBoard();
    void printBoardState() const;
    bool isFlopDealt() const;
    bool isTurnDealt() const;
    bool isRiverDealt() const;
};

#endif // BOARD_H
