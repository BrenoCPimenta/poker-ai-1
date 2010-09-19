#include <QtCore/QCoreApplication>
#include "deck.h"
#include <QDebug>
#include <QTime>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qsrand(QTime::currentTime().msec());

    Deck deck;
    deck.generate();
    deck.shuffle();
//    deck.printOut();
    qDebug() << "Gief strength:" << deck.strength();

//    return a.exec();
}
