#ifndef PREFLOP_H
#define PREFLOP_H

#include <QMap>
#include <QPair>

#include "card.h"
#include "deck.h"

typedef QPair<Card::Value, Card::Value> CardPair;

class PreFlop
{
public:
    PreFlop();


    static int calculateWinningProbability(const Deck &hand, int otherPlayers, int rollouts);

    void loadData(const QString &filename = "preflop.csv");
    void generateData(int rollouts = 5);
    void saveData(const QString &filename = "preflop.csv");

    bool isValid() { return !m_unpairedMixedSuit.isEmpty() && !m_unpairedSameSuit.isEmpty() && !m_paired.isEmpty(); }

    int getProbability(const Card &card1, const Card &card2, int players);

private:
    //   players         Card1 value  Card 2 value  prob
    QMap<int, QMap<CardPair, int> > m_unpairedMixedSuit;
    QMap<int, QMap<CardPair, int> > m_unpairedSameSuit;
    QMap<int, QMap<Card::Value, int> > m_paired;
};

#endif // PREFLOP_H
