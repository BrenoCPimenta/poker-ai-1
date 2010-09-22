#ifndef PLAYER_H
#define PLAYER_H

class Table;
#include "deck.h"

class Player
{
public:
    enum Action {
        Fold,
        Call,
        Raise
    };

    Player();

    virtual Action assess(Table *table);
    int handStrength(Table *table);

    void setDeck(Deck deck);

    bool hasFolded() const { return m_hasFolded; }

    int money() { return m_money; }
    void takeMoney(int amount) { m_money -= amount; }
    void giveMoney(int amount) { m_money += amount; }

    int bet() { return m_bet; }

    int lastHandStrength() { return m_lastHandStrength; }

protected:
    Deck m_cards;
    bool m_hasFolded;
    int m_money;
    int m_bet;

private:
    int m_lastHandStrength;
};

#endif // PLAYER_H
