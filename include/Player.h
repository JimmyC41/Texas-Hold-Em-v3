#ifndef PLAYER_H
#define PLAYER_H

#include "Card.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;

enum class Position {
    SMALL_BLIND = 0,
    BIG_BLIND,
    UTG,
    UTG_1,
    MIDDLE,
    LOJACK,
    HIJACK,
    CUT_OFF,
    BUTTON
};

const int NUM_POSITIONS = 9;

class Player {
private:
    string name;
    Position position;
    size_t chips;
    vector<Card> hand;
    void validateChipAmount(size_t amount) const;
public:
    Player(string name, Position position, size_t chips);

    void setPosition(Position position);
    void reduceChips(size_t chips);
    void addChips(size_t chips);
    void addHoleCard(const Card& card);

    Position getPosition() const;
    size_t getChips() const;
    const vector<Card>& getHand() const;
    void resetHand();
    string getName() const;

    static Position nextPosition(Position curPosition);
    static string positionToStr(Position position);
};

#endif // PLAYER_H