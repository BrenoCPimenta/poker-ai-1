#include "deck.h"
#include <QDebug>

Deck::Deck() : QList<Card>()
{
}

void Deck::generate()
{
    for (int s=0; s<4; s++) {
        for (int v=0; v<13; v++) {
            append(Card((Card::Value)v, (Card::Suit)s));
        }
    }
}

void Deck::shuffle(int iterations)
{
    for (int i=0; i<iterations; i++) {
        for (int j=1; j<size(); j++) {
            Card c = takeAt(qrand() % j);
            append(c);
        }
        qWarning() << i;
    }
}

int Deck::strength()
{
    Deck deck(*this);

    if (!hasStraightFlush(deck))
        return 9;
    else if (!hasFourOfAKind(deck))
        return 8;
    else if (!hasFullHouse(deck))
        return 7;
    else if (!hasFlush(deck))
        return 6;
    else if (hasStraight(deck))
        return 5;

    return 0;
}

bool Deck::hasStraightFlush(Deck deck)
{
    // Cards are sorted first by suit, then by value
    qSort(deck.begin(), deck.end(), suitValueCompare);
    Card::Suit curSuit = deck.first().suit();
    int count = 0;
    Card::Value lastValue = (Card::Value)-1;

    foreach(const Card &card, deck) {
        if (curSuit == card.suit() && card.value() == (lastValue + 1)) {
            count++;
            if (count >= 5)
                return true;
        } else {
            curSuit = card.suit();
            count = 0;
        }
        lastValue = card.value();
    }

    return false;
}

bool Deck::hasFourOfAKind(Deck deck)
{
    // Sorts only by card value
    qSort(deck.begin(), deck.end(), valueCompare);
    deck.printOut();
    Card::Value lastValue = (Card::Value)-1;
    int count = 0;

    foreach(const Card &card, deck) {
        if (lastValue == card.value()) {
            count++;
            if (count >= 4)
                return true;
        } else {
            count = 1;
            lastValue = card.value();
        }
    }
    return false;
}

bool Deck::hasFullHouse(Deck deck)
{
    int counts[13];
    memset(counts, 0, sizeof(int) * 13);

    foreach(const Card &card, deck)
        counts[card.value()]++;
    int threes, twos;

    threes = twos = 0;

    for (int i=0; i<13; i++) {
        if (counts[i] >= 3) {
            if (threes || twos)
                return true;
            else
                threes++;
        }
        else if (counts[i] >= 2) twos++;
    }

    return false;

}

bool Deck::hasFlush(Deck deck)
{
    // Cards are sorted first by suit, then by value
    qSort(deck.begin(), deck.end(), suitCompare);
    Card::Suit curSuit = (Card::Suit)-1;
    int count = 0;

    foreach(const Card &card, deck) {
        if (curSuit == card.suit()) {
            count++;
            if (count >= 5)
                return true;
        } else {
            count = 1;
            curSuit = card.suit();
        }
    }

    return false;
}

bool Deck::hasStraight(Deck deck)
{
    // Sort by value only
    qSort(deck.begin(), deck.end(), suitValueCompare);

    Card::Value lastValue = deck.first().value();
    int count = 0;

    foreach(const Card &card, deck) {
        if (card.value() == (lastValue + 1)) {
            count++;
            if (count >= 5) return true;
        } else {
            count = 1;
        }
        lastValue = card.value();
    }
    return false;
}

bool Deck::hasThreeOfAKind(Deck deck)
{

}

void Deck::printOut()
{
    foreach(const Card &card, *this) {
        qDebug() << card.toString();
    }
}

/**
  * Sorts first by suit, then by value.
  */
bool Deck::suitValueCompare(const Card &one, const Card &two)
{
    if (one.suit() != two.suit())
        return one.suit() < two.suit();
    else
        return one.value() < two.value();
}

/**
  * Sorts simply by value.
  */
bool Deck::valueCompare(const Card &one, const Card &two)
{
    return one.value() < two.value();
}

/**
  * Sorts simply by suit.
  */
bool Deck::suitCompare(const Card &one, const Card &two)
{
    return one.suit() < two.suit();
}

Deck Deck::take(int num)
{
    Deck deck;
    for (int i=0; i<num; i++)
        deck.append(takeLast());

    return deck;
}
