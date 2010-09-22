#include "deck.h"

#include "card.h"
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
    }
}

int Deck::strength()
{
    Deck deck(*this);

    if (hasStraightFlush(deck))
        return 9;
    else if (hasFourOfAKind(deck))
        return 8;
    else if (hasFullHouse(deck))
        return 7;
    else if (hasFlush(deck))
        return 6;
    else if (hasStraight(deck))
        return 5;
    else if (hasThreeOfAKind(deck))
        return 4;
    else if (hasTwoPair(deck))
        return 3;
    else if (hasPair(deck))
        return 2;
    else
        return 1;
}

bool Deck::hasStraightFlush(Deck deck)
{
    // Cards are sorted first by suit, then by value
    qSort(deck.begin(), deck.end(), suitValueCompare);
    Card::Suit curSuit = deck.first().suit();
    int count = 1;
    Card::Value lastValue = deck.first().value();

    foreach(const Card &card, deck) {
        if (curSuit == card.suit() && card.value() == (lastValue + 1)) {
            count++;
            if (count >= 5)
                return true;
        } else {
            curSuit = card.suit();
            count = 1;
        }
        lastValue = card.value();
    }

    return false;
}

bool Deck::hasFourOfAKind(Deck deck)
{
    // Sorts only by card value
    qSort(deck.begin(), deck.end(), valueCompare);
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
    bool threes = false, twos = false;


    for (int i=0; i<13; i++) {
        if (counts[i] >= 3) {
            if (threes || twos) {
                return true;
            } else {
                threes = true;
            }
        } else if (counts[i] >= 2) {
            twos = true;
        }
    }

    return (threes && twos);

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
    qSort(deck.begin(), deck.end(), valueCompare);

    Card::Value lastValue = deck.first().value();
    int count = 1;

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
    qSort(deck.begin(), deck.end(), valueCompare);
    Card::Value value = deck.first().value();
    int count = 0;

    foreach(const Card &card, deck) {
        if (card.value() == value) {
            count++;
            if (count >= 3) return true;
        } else {
            count = 1;
            value = card.value();
        }
    }
    return false;
}

bool Deck::hasTwoPair(Deck deck)
{
    qSort(deck.begin(), deck.end(), valueCompare);

    Card::Value lastValue = (Card::Value)-1;
    bool hasOnePair = false;

    foreach(const Card &card, deck) {
        if (card.value() == lastValue) {
            if (hasOnePair)
                return true;
            else
                hasOnePair = true;
        }
        lastValue = card.value();
    }

    return false;
}

bool Deck::hasPair(Deck deck)
{
    qSort(deck.begin(), deck.end(), valueCompare);

    Card::Value lastValue = (Card::Value)-1;

    foreach(const Card &card, deck) {
        if (card.value() == lastValue)
            return true;
        lastValue = card.value();
    }

    return false;
}

void Deck::printOut() const
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

// Checks if this deck looses against another one
bool Deck::operator <(Deck other)
{
    int s = strength(), o = other.strength();
    if (s!=o)
        return s < o;

    Deck deck(*this); // Copy that floppy

    if (hasStraightFlush(deck) || hasFlush(deck)) {        
        qSort(deck.begin(), deck.end(), suitValueCompare);
        qSort(other.begin(), other.end(), suitValueCompare);
        return compareDecks(deck, other);
    } else if (hasFourOfAKind(deck) || hasFullHouse(deck) || hasStraight(deck)) {
        qSort(deck.begin(), deck.end(), valueCompare);
        qSort(other.begin(), other.end(), valueCompare);
        return compareDecks(deck, other);
    } else if (hasThreeOfAKind(deck)) {
        Deck one, two;
        one << getMostValue(deck)
            << getSecondMostValue(deck);
        two << getMostValue(other)
            << getSecondMostValue(other);
        return compareDecks(one, two);
    } else if (hasTwoPair(deck) || hasPair(deck)) {
        qSort(deck.begin(), deck.end(), valueCompare);
        qSort(other.begin(), other.end(), valueCompare);

        deck = getPairs(deck);
        other = getPairs(other);
        qDebug() << deck.size() << ":" << other.size();

        return compareDecks(deck, other);
    } else {
        qSort(deck.begin(), deck.end(), valueCompare);
        qSort(other.begin(), other.end(), valueCompare);
        return compareDecks(deck, other);
    }
}

bool Deck::compareDecks(const Deck &one, const Deck &two)
{
    for (int i=0; i<one.size() && i<two.size(); i++) {
        if (one[i].value() != two[i].value())
            return one[i].value() < two[i].value();
    }

    return false;
}

Card::Value Deck::getMostValue(const Deck &deck)
{
    QMap<Card::Value, int> valueCounts;
    for (int i=0; i<(int)Card::Ace; i++)
        valueCounts[(Card::Value)i] = 0;
    foreach(const Card card, deck) {
        valueCounts[card.value()]++;
    }

    int max = -1;
    Card::Value value = (Card::Value)-1;
    for (int i=0; i<Card::Ace; i++) {
        if (valueCounts[(Card::Value)i] > max) {
            max = valueCounts[(Card::Value)i];
            value = (Card::Value)i;
        }
    }

    return value;
}

Card::Value Deck::getSecondMostValue(const Deck &deck)
{
    QMap<Card::Value, int> valueCounts;
    for (int i=0; i<(int)Card::Ace; i++)
        valueCounts[(Card::Value)i] = 0;
    foreach(const Card card, deck) {
        valueCounts[card.value()]++;
    }

    int max = -1;
    Card::Value mostValue = (Card::Value)-1;
    Card::Value secondMost = (Card::Value)-1;
    for (int i=0; i<Card::Ace; i++) {
        if (valueCounts[(Card::Value)i] > max) {
            max = valueCounts[(Card::Value)i];
            secondMost = mostValue;
            mostValue = (Card::Value)i;
        }
    }

    return secondMost;
}

Deck Deck::getPairs(const Deck &deck)
{
    Deck d;
    Card old;
    foreach(const Card &card, deck) {
        if (card.value() == old.value()) {
            d << card
              << old;
        }

        old = card;
    }
    return d;
}

void Deck::removeCards(const Deck &hand){
    // Take out the cards on the hand
    foreach(const Card &card, hand) {
        for (int j=0; j<size(); j++) {
            if (at(j).value() == card.value() && at(j).suit() == card.suit()) {
                takeAt(j);
                break;
            }
        }
    }
}
