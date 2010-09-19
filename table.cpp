#include "table.h"
#include "player.h"

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
    m_turn = Card(); // null card
    m_river = Card();

    Deck deck;
    deck.generate();
    deck.shuffle();

    for (int i=0; i<m_players.size(); i++) {
        m_players[i].setDeck(deck.take(2));
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

    for (int i=0; i<m_players.size(); i++) {
        if (!m_players[i].hasFolded() && m_players[i].handStrength(this) > strongestHand) {
            strongestHand = m_players[i].lastHandStrength();
        }
    }

    int count = 0;
    for (int i=0; i<m_players.size(); i++) {
        if (!m_players[i].hasFolded() && m_players[i].lastHandStrength() == strongestHand)
            count++;
    }

    int potPayout = m_pot / count;
    for (int i=0; i<m_players.size(); i++) {
        if (!m_players[i].hasFolded() && m_players[i].lastHandStrength() == strongestHand)
            m_players[i].giveMoney(potPayout);
    }
}

void Table::doBettingRound()
{
    Player::Action action;

    for (int i=0; i<m_players.size(); i++) {
        if (!m_players[i].hasFolded()) {
            action = m_players[i].assess(this);
            switch(action) {
            case (Player::Fold):
                continue;
            case (Player::Call):
                m_pot += m_lastBet;
                m_players[i].takeMoney(m_lastBet);
                break;
            case (Player::Raise):
                m_pot += m_players[i].bet();
                m_players[i].takeMoney(m_players[i].bet());
            }
        }
    }
}

int Table::activePlayers()
{
    int c=0;
    for (int i=0; i<m_players.size(); i++) {
        if (!m_players[i].hasFolded())
            c++;
    }
    return c;
}
