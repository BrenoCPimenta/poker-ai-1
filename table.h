#ifndef TABLE_H
#define TABLE_H

#include <QObject>
#include "card.h"
#include "deck.h"
#include "player.h"

class Player;
class Deck;
class Card;

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
    int numPlayers() { return m_players.size(); }

    void playHand();

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
