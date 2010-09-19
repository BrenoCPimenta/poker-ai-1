#include "table.h"

Table::Table(QObject *parent) :
        QObject(parent),
        m_pot(0),
        m_lastBet(0)
{
}

void Table::play()
{
    m_lastBet = 0;
    m_pot = 0;
    m_flop.clear();
    m_turn = Card; // null card
    m_river = Card;

    Deck deck;
    deck.generate();
    deck.shuffle();

    foreach (const Player &player, m_players) {
        player.setDeck(deck.take(2));
    }

    doBettingRound();

    deck.takeLast(); // burn card
    m_flop = deck.take(3);
    doBettingRound();

    deck.takeLast(); // burn card
    m_turn = deck.takeLast();
    doBettingRound();

    deck.takeLast(); // burn card
    m_river = deck.takeLast();
    doBettingRound();

    int strongestHand = -1;

    foreach(const Player &player, m_players) {
        if (!player.hasFolded() && player.calculateHandStrength(this) > strongestHand) {
            strongestHand = player.lastHandStrength();
        }
    }

    int count = 0;
    foreach(const Player &player, m_players) {
        if (!player.hasFolded() && player.lastHandStrength() == strongestHand)
            count++;
    }

    int potPayout = m_pot / count;
    foreach(const Player &player, m_players) {
        if (!player.hasFolded() && player.lastHandStrength() == strongestHand)
            player.giveMoney(potPayout);
    }
}

void Table::doBettingRound()
{
    Player::Action action;

    foreach(const Player &player, m_players) {
        if (!player.hasFolded()) {
            action = player.assess(this);
            switch(action) {
            case (Player::Fold):
                continue;
            case (Player::Call):
                m_pot += m_lastBet;
                player.takeMoney(m_lastBet);
                break;
            case (Player::Raise):
                m_pot += player.bet();
                player.takeMoney(player.bet());
            }
        }
    }
}

int Table::activePlayers()
{
    int c=0;
    foreach(const Player &player, m_players) {
        if (!player.hasFolded())
            c++;
    }
    return c;
}
