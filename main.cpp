#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QTime>
#include <QStringList>

#include "table.h"
#include "preflop.h"

#include "deck.h"

#include "test.cpp"

int main(int argc, char *argv[])
{
    testDeck();
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
        }
    }

    Table table;

    table.play(1000);
//    return a.exec();
}

