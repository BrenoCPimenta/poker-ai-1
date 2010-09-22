#include "deck.h"

void testDeck()
{
    qDebug() << "Testan tiem!";

    Deck deck, highCard;
    highCard << Card(Card::Three, Card::Club)
            << Card(Card::Two, Card::Heart)
            << Card(Card::Four, Card::Diamond)
            << Card(Card::King, Card::Club)
            << Card(Card::Eight, Card::Spade);

    // Test straight flush
    deck << Card(Card::Two, Card::Spade)
            << Card(Card::Three, Card::Spade)
            << Card(Card::Four, Card::Spade)
            << Card(Card::Five, Card::Spade)
            << Card(Card::Six, Card::Club);
    Q_ASSERT(!Deck::hasStraightFlush(deck));

    Deck straightFlush;
    straightFlush << Card(Card::Two, Card::Spade)
            << Card(Card::Three, Card::Spade)
            << Card(Card::Four, Card::Spade)
            << Card(Card::Five, Card::Spade)
            << Card(Card::Six, Card::Spade);
    Q_ASSERT(Deck::hasStraightFlush(straightFlush));

    // Test four of a kind
    deck.clear();
    deck << Card(Card::Two, Card::Spade)
            << Card(Card::Two, Card::Club)
            << Card(Card::Two, Card::Diamond)
            << Card(Card::Three, Card::Heart)
            << Card(Card::Six, Card::Spade);
    Q_ASSERT(!Deck::hasFourOfAKind(deck));

    Deck fourOfAKind;
    fourOfAKind << Card(Card::Two, Card::Spade)
            << Card(Card::Two, Card::Club)
            << Card(Card::Two, Card::Diamond)
            << Card(Card::Two, Card::Heart)
            << Card(Card::Six, Card::Spade);
    Q_ASSERT(Deck::hasFourOfAKind(fourOfAKind));

    // Test full house
    deck.clear();
    deck << Card(Card::Two, Card::Spade)
            << Card(Card::Two, Card::Club)
            << Card(Card::Two, Card::Diamond)
            << Card(Card::Two, Card::Heart)
            << Card(Card::Six, Card::Spade);
    Q_ASSERT(!Deck::hasFullHouse(deck));

    Deck fullHouse;
    fullHouse << Card(Card::Three, Card::Club)
            << Card(Card::Three, Card::Spade)
            << Card(Card::Three, Card::Diamond)
            << Card(Card::Six, Card::Club)
            << Card(Card::Six, Card::Heart);
    Q_ASSERT(Deck::hasFullHouse(fullHouse));

    // Test flush
    deck.clear();
    deck << Card(Card::Queen, Card::Club)
            << Card(Card::Ten, Card::Club)
            << Card(Card::Seven, Card::Club)
            << Card(Card::Six, Card::Club)
            << Card(Card::Four, Card::Diamond);
    Q_ASSERT(!Deck::hasFlush(deck));

    Deck flush;
    flush << Card(Card::Queen, Card::Club)
            << Card(Card::Ten, Card::Club)
            << Card(Card::Seven, Card::Club)
            << Card(Card::Six, Card::Club)
            << Card(Card::Four, Card::Club);
    Q_ASSERT(Deck::hasFlush(flush));

    //Test straight
    deck.clear();
    deck << Card(Card::Queen, Card::Club)
            << Card(Card::Jack, Card::Spade)
            << Card(Card::Ten, Card::Spade)
            << Card(Card::Nine, Card::Heart)
            << Card(Card::Ten, Card::Heart);
    Q_ASSERT(!Deck::hasStraight(deck));

    Deck straight;
    straight << Card(Card::Queen, Card::Club)
            << Card(Card::Jack, Card::Spade)
            << Card(Card::Ten, Card::Spade)
            << Card(Card::Nine, Card::Heart)
            << Card(Card::Eight, Card::Heart);
    Q_ASSERT(Deck::hasStraight(straight));

    //Test three of a kind
    deck.clear();
    deck << Card(Card::Two, Card::Diamond)
            << Card(Card::Two, Card::Spade)
            << Card(Card::Six, Card::Club)
            << Card(Card::King, Card::Spade)
            << Card(Card::Six, Card::Heart);
    Q_ASSERT(!Deck::hasThreeOfAKind(deck));

    Deck threeOfAKind;
    threeOfAKind << Card(Card::Two, Card::Diamond)
            << Card(Card::Two, Card::Spade)
            << Card(Card::Two, Card::Club)
            << Card(Card::King, Card::Spade)
            << Card(Card::Six, Card::Heart);
    Q_ASSERT(Deck::hasThreeOfAKind(threeOfAKind));

    Deck threeOfAKind2;
    threeOfAKind2 << Card(Card::Ten, Card::Heart)
                  << Card(Card::Ten, Card::Club)
                  << Card(Card::Four, Card::Diamond)
                  << Card(Card::Four, Card::Heart)
                  << Card(Card::Nine, Card::Heart);
    Q_ASSERT(threeOfAKind2 < threeOfAKind);

    //Test two pair
    deck.clear();
    deck << Card(Card::Jack, Card::Heart)
            << Card(Card::Jack, Card::Club)
            << Card(Card::Four, Card::Club)
            << Card(Card::Five, Card::Spade)
            << Card(Card::Nine, Card::Heart);
    Q_ASSERT(!Deck::hasTwoPair(deck));

    Deck twoPair;
    twoPair << Card(Card::Jack, Card::Heart)
            << Card(Card::Jack, Card::Club)
            << Card(Card::Four, Card::Club)
            << Card(Card::Four, Card::Spade)
            << Card(Card::Nine, Card::Heart);
    Q_ASSERT(!Deck::hasThreeOfAKind(twoPair));

    Deck twoPair2;
    twoPair2 << Card(Card::Ten, Card::Heart)
            << Card(Card::Ten, Card::Club)
            << Card(Card::Four, Card::Diamond)
            << Card(Card::Four, Card::Heart)
            << Card(Card::Nine, Card::Heart);
    Q_ASSERT(twoPair2 < twoPair);


    //Test one pair
    deck.clear();
    deck << Card(Card::Four, Card::Heart)
            << Card(Card::Eight, Card::Spade)
            << Card(Card::King, Card::Spade)
            << Card(Card::Ten, Card::Diamond)
            << Card(Card::Five, Card::Spade);
    Q_ASSERT(!Deck::hasTwoPair(deck));

    Deck onePair;
    onePair << Card(Card::Four, Card::Heart)
            << Card(Card::Four, Card::Spade)
            << Card(Card::King, Card::Spade)
            << Card(Card::Ten, Card::Diamond)
            << Card(Card::Five, Card::Spade);
    Q_ASSERT(!Deck::hasTwoPair(onePair));

    Deck onePair2;
    onePair2 << Card(Card::Three, Card::Heart)
             << Card(Card::Three, Card::Spade)
             << Card(Card::King, Card::Spade)
             << Card(Card::Ten, Card::Spade)
             << Card(Card::Six, Card::Spade);
    Q_ASSERT(onePair2 < onePair);
}

