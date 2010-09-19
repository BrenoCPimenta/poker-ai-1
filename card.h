#ifndef CARD_H
#define CARD_H

#include <QList>

class Card
{
public:
    enum Suit {
        Spade = 0,
        Heart,
        Club,
        Diamond
    };

    enum Value {
        Two = 0,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King,
        Ace
    };

    Card(Value value = (Value)-1, Suit suit = (Suit)-1);
    Card(const Card &card);

    Suit suit() const { return m_suit; }
    Value value() const { return m_value; }

    bool operator <(const Card &other) const;

    bool isNull() { return (m_value == -1 && m_suit == -1); }

    QString toString() const;

private:
    Value m_value;
    Suit m_suit;
};

#endif // CARD_H
