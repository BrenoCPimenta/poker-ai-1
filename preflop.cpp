#include "preflop.h"
#include "deck.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>




PreFlop::PreFlop()
{
}

/**
  * Returns number of wins per 1000, given a hand
  */
int PreFlop::calculateWinningProbability(const Deck &hand, int otherPlayers, int rollouts)
{

    otherPlayers--; // subtract ourselves

    int wins = 0, ties = 0;
    QList<Deck> players;
    for (int i=0; i<otherPlayers; i++)
        players << Deck();

    bool tie, win;
    Deck community, opponent, my;
    for (int round=0; round<rollouts; round++) {
        Deck deck;
        deck.generate();

        // Take out the cards on the hand
        foreach(const Card &card, hand) {
            for (int j=0; j<deck.size(); j++) {
                if (deck[j].value() == card.value() && deck[j].suit() == card.suit()) {
                    deck.takeAt(j);
                    break;
                }
            }
        }

        deck.shuffle();

        community = deck.take(5);

        win = true;
        tie = false;
        for (int j=0; j<otherPlayers; j++) {
            opponent = community;
            opponent.append(deck.take(2));
            my = community;
            my.append(hand);
            if (win && opponent < my) {
                win = true;
            } else if (!(my < opponent)) {
                tie = true;
            } else {
                win = false;
            }
        }

        if (win) {
            wins++;
        } else if (tie) {
            ties++;
        }

    }
    wins += ties/2; // Sane? Why I would never.
    wins /= (1000/rollouts);
    return wins;
}

void PreFlop::generateData(int rollouts)
{
    Card card1, card2;
    Deck deck;
    for (int players = 2; players<=10; players++) {
        qDebug() << "Generating for"<<players<<"players";
        //Generate mixed suit data
        for (int value1=0; value1<=13; value1++) {
            for (int value2=value1; value2<=13; value2++) {
                card1.set((Card::Value)value1, Card::Heart);
                card2.set((Card::Value)value2, Card::Club);
                deck.clear();
                deck << card1 << card2;
                m_unpairedMixedSuit[players][CardPair((Card::Value)value1, (Card::Value)value2)] = calculateWinningProbability(deck, players, rollouts);
            }
        }

        //Generate same suit data
        for (int value1=0; value1<=13; value1++) {
            for (int value2=value1; value2<=13; value2++) {
                card1.set((Card::Value)value1, Card::Heart);
                card2.set((Card::Value)value2, Card::Heart);
                deck.clear();
                deck << card1 << card2;
                m_unpairedSameSuit[players][CardPair((Card::Value)value1, (Card::Value)value2)] = calculateWinningProbability(deck, players, rollouts); // lolsamedrakt
            }
        }

        for (int value=0; value<=13; value++) {
            card1.set((Card::Value)value, Card::Heart);
            card2.set((Card::Value)value, Card::Club);
            deck.clear();
            deck << card1 << card2;
            m_paired[players][(Card::Value)value] = calculateWinningProbability(deck, players, rollouts);
        }
    }
}

void PreFlop::saveData(const QString &filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    // Write out stats for unpaired, mixed suit cards
    foreach(int players, m_unpairedMixedSuit.keys()) {
        foreach(const CardPair &pair, m_unpairedMixedSuit[players].keys()) {
            out << "Mixed," << players << ","
                    << pair.first << ","
                    << pair.second << ","
                    << m_unpairedMixedSuit[players][pair] << "\n";
        }
    }

    // Write out stats for unpaired, same suit cards
    foreach(int players, m_unpairedSameSuit.keys()) {
        foreach(const CardPair &pair, m_unpairedSameSuit[players].keys()) {
            out << "Same," << players << ","
                    << pair.first << ","
                    << pair.second << ","
                    << m_unpairedSameSuit[players][pair] << "\n";
        }
    }
    // Write out stats for pairs
    foreach(int players, m_paired.keys()) {
        foreach(Card::Value val, m_paired[players].keys()) {
            out << "Pair," << players << ","
                    << val << ","
                    << m_paired[players][val] << "\n";
        }
    }
}

void PreFlop::loadData(const QString &filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QList<QByteArray> line;
    Card::Value val1, val2;
    int players, possibility;
    while (!file.atEnd()) {
        line = file.readLine().split(',');
        qDebug() << line;
        if (line[0] == "Mixed") {
            players = atoi(line[1]);
            val1 = (Card::Value)atoi(line[2]);
            val2 = (Card::Value)atoi(line[3]);
            possibility = atoi(line[4]);
            m_unpairedMixedSuit[players][CardPair(val1, val2)] = possibility;
        } else if (line[0] == "Same") {
            players = atoi(line[1]);
            val1 = (Card::Value)atoi(line[2]);
            val2 = (Card::Value)atoi(line[3]);
            possibility = atoi(line[4]);
            m_unpairedSameSuit[players][CardPair(val1, val2)] = possibility;
        } else if (line[0] == "Pair") {
            players = atoi(line[1]);
            val1 = (Card::Value)atoi(line[2]);
            possibility = atoi(line[3]);
            m_paired[players][val1] = possibility;
        }
    }
}

int PreFlop::getProbability(const Card &card1, const Card &card2, int players)
{
    if (!isValid())
        return -1;

    if (card1.suit() == card2.suit()) {
        return m_unpairedSameSuit[players][CardPair(card1.value(), card2.value())];
    } else if (card1.value() == card2.value()) {
        return m_paired[players][card1.value()];
    } else {
        return m_unpairedMixedSuit[players][CardPair(card1.value(), card2.value())];
    }
}
