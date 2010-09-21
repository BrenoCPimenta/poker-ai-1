#include <QtCore/QCoreApplication>
#include "table.h"
#include <QDebug>
#include <QTime>
#include <QStringList>

#include "deck.h"

#define ROLLOUTS 1000

/**
  * Returns number of wins per ROLLOUTS, given a hand
  */
int getWinningProbability(Deck hand, int otherPlayers)
{
    int wins = 0, ties = 0;
    QList<Deck> players;
    for (int i=0; 0<otherPlayers; i++)
        players << Deck();

    bool tie, win;
    Deck community, opponent, my;
    for (int i=0; i<ROLLOUTS; i++) {
        Deck deck;
        deck.generate();

        // Take out the cards on the hand
        foreach(const Card &card, hand) {
            for (int j=0; j<deck.size(); j++) {
                if (deck[i].value() == card.value() && deck[i].suit() == card.suit()) {
                    deck.takeAt(i);
                    break;
                }
            }
        }

        community = deck.take(5);

        win = true;
        tie = false;
        for (int j=0; j<otherPlayers; j++) {
            opponent = community;
            opponent.append(deck.take(2));
            my = community;
            my.append(hand);
            if (win && opponent < my)
                win = true;
            else if (!(my < opponent))
                tie = true;
            else win = false;
        }
        if (win)
            wins++;
        else if (tie)
            ties++;
    }
    return wins + (ties / 2);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qsrand(QTime::currentTime().msec());

    if (a.arguments().contains("-rollout")) {
        QString filename;

        {
            int i= a.arguments().indexOf("-rollout");
            if (i != a.arguments().size() - 2) {
                qWarning() << "usage: " << a.arguments()[0]
                        << " [-rollout filename]\t - writes rollout data to filename";
                exit(0);
            }
            filename = a.arguments()[++i];
            Deck hand;
            hand << Card(Card::Ace, Card::Heart)
                 << Card(Card::Ace, Card::Club);
            qWarning << getWinningProbability(hand, 5);
        }
        qDebug() << filename;


    }

//    Table table;

//    table.playHand();
//    return a.exec();
}

