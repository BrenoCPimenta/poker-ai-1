#ifndef DECK_H
#define DECK_H

#include <QList>

#include "card.h"

class Deck : public QList<Card>
{
public:
    Deck();
    void generate();
    void shuffle(int iterations = 5);

    int strength();
    Deck take(int num);

    void sort();

    void printOut();

    bool operator <(Deck other);

    static bool hasStraightFlush(Deck deck);
    static bool hasFourOfAKind(Deck deck);
    static bool hasFullHouse(Deck deck);
    static bool hasFlush(Deck deck);
    static bool hasStraight(Deck deck);
    static bool hasThreeOfAKind(Deck deck);
    static bool hasTwoPair(Deck deck);
    static bool hasPair(Deck deck);


private:

   static Card::Value getMostValue(const Deck &deck);
   static Card::Value getSecondMostValue(const Deck &deck);

   static Deck getPairs(const Deck &deck);

   static inline bool suitCompare(const Card &a, const Card &b);
   static inline bool valueCompare(const Card &a, const Card &b);
   static inline bool suitValueCompare(const Card &a, const Card &b);

   static inline bool compareDecks(const Deck &one, const Deck &two);
};

#endif // DECK_H
