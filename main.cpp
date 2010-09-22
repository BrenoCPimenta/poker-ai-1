// 10:30, 361 ITV

#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QTime>
#include <QStringList>

#include "table.h"
#include "preflop.h"
#include "preflopplayer.h"

#include "deck.h"

#include "test.cpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qsrand(QTime::currentTime().msec());

    if (a.arguments().size() > 1) {
        if (a.arguments()[1] == ("-g")) {
            QString filename;

            if (a.arguments().size() > 2) {
                filename = a.arguments()[2];
                qDebug() << "Writing preflop data to" << filename;
            }

            PreFlop preflop;
            preflop.generateData(1000);

            if (filename.isEmpty())
                preflop.saveData();
            else
                preflop.saveData(filename);
        } else if (a.arguments()[1] == "-t") {
            testDeck();
        } else if (a.arguments()[1] == "-h") {
            RolloutPlayer player;
            Deck deck;
            deck.generate();
            deck.shuffle();
            Deck straightFlush;
            straightFlush << Card(Card::Two, Card::Spade)
                    << Card(Card::Three, Card::Spade);
            qDebug() << "Cards on hand:";
            straightFlush.printOut();

            Deck community;
            community << Card(Card::Four, Card::Spade)
                    << Card(Card::Five, Card::Spade)
                    << Card(Card::Six, Card::Spade);
            qDebug() << "\nCommunity cards:";
            community.printOut();

            deck.removeCards(straightFlush);

            qDebug() << "Strength:" << player.h(straightFlush, community, 5);
        } else {
            qWarning() << "usage: " << a.arguments()[0] << " [-t|-g|-h]\n"
                    << "\t-t\tTest card deck\n"
                    << "\t-g\tGenerate preflop data (1000 rounds)\n"
                    << "\t-h\tTest hand strength algorithm\n";
        }
    } else {
        Table table(Table::II);
        table.play(1000);
    }
}
