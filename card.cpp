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

Card Card::fromString(const QString &string)
{
    Suit suit;
    Value value;
    switch(string[0].toAscii()) {
    case 'S':
        suit = Card::Spade;
        break;
    case 'H':
        suit = Card::Heart;
        break;
    case 'C':
        suit = Card::Club;
        break;
    case 'D':
        suit = Card::Diamond;
        break;
    default:
        suit = (Card::Suit)-1;
    }

    switch(string[1].toAscii()) {
    case '2':
        value = Card::Two;
        break;
    case '3':
        value = Card::Three;
        break;
    case '4':
        value = Card::Four;
        break;
    case '5':
        value = Card::Five;
        break;
    case '6':
        value = Card::Six;
        break;
    case '7':
        value = Card::Seven;
        break;
    case '8':
        value = Card::Eight;
        break;
    case '9':
        value = Card::Nine;
        break;
    case '0':
        value = Card::Ten;
        break;
    case 'J':
        value = Card::Jack;
        break;
    case 'Q':
        value = Card::Queen;
        break;
    case 'K':
        value = Card::King;
        break;
    case 'A':
        value = Card::Ace;
        break;
    default:
        value = (Card::Value)-1;
    }

    return Card(value, suit);
}
