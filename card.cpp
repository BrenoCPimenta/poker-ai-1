#include "card.h"
#include <QChar>
#include <QString>

Card::Card(Value value, Suit suit) :
        m_value(value),
        m_suit(suit)
{
}

Card::Card(const Card &card) :
        m_value(card.value()),
        m_suit(card.suit())
{
}

QString Card::toString() const
{
    QString r;

    switch (m_suit) {
    case Spade:
//        r = "♠";
        r = "S";
        break;
    case Heart:
        //r = "♥";
        r = "H";
        break;
    case Club:
        //r = "♣";
        r = "C";
        break;
    case Diamond:
        //r = "♦";
        r = "D";
        break;
    default:
        r = " ";
    }

    switch (m_value) {
    case Two:
        r += "2";
        break;
    case Three:
        r += "3";
        break;
    case Four:
        r += "4";
        break;
    case Five:
        r += "5";
        break;
    case Six:
        r += "6";
        break;
    case Seven:
        r += "7";
        break;
    case Eight:
        r += "8";
        break;
    case Nine:
        r += "9";
        break;
    case Ten:
        r += "0";
        break;
    case Jack:
        r += "J";
        break;
    case Queen:
        r += "Q";
        break;
    case King:
        r += "K";
        break;
    case Ace:
        r += "A";
        break;
    }

    return r;
}
