#ifndef TABLE_H
#define TABLE_H

#include "card.h"
#include "deck.h"
#include "player.h"
#include "statisticalmodel.h"

#include <QMap>


class Player;
class Deck;
class Card;

class Table
{
public:
    enum Phase {
        I,
        II,
        III
    };

    Table(Phase phase);

    Deck flop() { return m_flop; }
    Card turn() { return m_turn; }
    Card river() { return m_river; }

    int pot() { return m_pot; }
    int lastBet() { return m_lastBet; }
    int numPlayers() { return m_players.size(); }

    int activePlayers();

    void play(int rounds);

private:
    void playHand();
    void doBettingRound();

    QList<Player> m_players;

    Deck m_flop;
    Card m_turn;
    Card m_river;

    int m_pot;
    int m_lastBet;
    Phase m_phase;

    // Username <> Statistical model
    QMap<QString, StatisticalModel> m_models;
};

#endif // TABLE_H
