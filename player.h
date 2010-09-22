#ifndef PLAYER_H
#define PLAYER_H

class Table;
#include "deck.h"
#include <QString>

class Player
{
public:
    enum Action {
        Fold,
        Call,
        Raise
    };

    Player(bool offensive = false);

    virtual Action assess(Table *table);
    int handStrength(Table *table);

    void setDeck(Deck deck);

    bool hasFolded() const { return m_hasFolded; }

    int money() { return m_money; }
    void takeMoney(int amount) { m_money -= amount; }
    void giveMoney(int amount) { m_money += amount; }

    int bet() { return m_bet; }

    int lastHandStrength() { return m_lastHandStrength; }

    const QString name() { return m_name; }


    void win() { m_win++; }
    int wins() { return m_win; }

    void tie() { m_tie++; }
    int ties() { return m_tie; }

    int bets;
    const Deck &deck() { return m_cards; }

protected:
    Deck m_cards;
    bool m_hasFolded;
    int m_money;
    int m_bet;

    bool m_offensive;
    QString m_name;

private:
    int m_lastHandStrength;
    int m_win;
    int m_tie;
};

#endif // PLAYER_H
