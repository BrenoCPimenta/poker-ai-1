#ifndef TABLE_H
#define TABLE_H

#include <QObject>

#include "player.h"
#include "deck.h"
#include "card.h"

class Table : public QObject
{
    Q_OBJECT
public:
    Table(QObject *parent = 0);

    Deck flop() { return m_flop; }
    Card turn() { return m_turn; }
    Card river() { return m_river; }

    int pot() { return m_pot; }
    int lastBet() { return m_lastBet; }

    void play();

private:
    void doBettingRound();
    int activePlayers();

    QList<Player> m_players;

    Deck m_flop;
    Card m_turn;
    Card m_river;

    int m_pot;
    int m_lastBet;
};

#endif // TABLE_H