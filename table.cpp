#include "table.h"
#include "player.h"

#include <QDebug>

Table::Table(QObject *parent) :
        QObject(parent),
        m_pot(0),
        m_lastBet(0)
{
    m_players << Player()
              << Player()
              << Player();
}

void Table::playHand()
{
    qDebug() << "Initiating hand.";
    m_lastBet = 0;
    m_flop.clear();
    m_turn = Card(); // null card
    m_river = Card();

    Deck deck;
    deck.generate();
    deck.shuffle();

    // Blinds
    m_pot = 150;
    m_players[0].takeMoney(50); // little blind
    m_players[1].takeMoney(100); // big blind
    m_lastBet = 100;

    for (int i=0; i<m_players.size(); i++) {
        qDebug() << "Giving player" << i << "a deck...";
        m_players[i].setDeck(deck.take(2));
        qDebug() << "Player" << i << "has" << m_players[i].money() << "monies.";
    }


    // Initial betting round
    doBettingRound();

    // Flop
    qDebug() << "Burn card before flop:" << deck.takeLast().toString(); // burn card
    m_flop = deck.take(3);
    qDebug() << "Flop:";
    m_flop.printOut();
    if (activePlayers() > 1)
        doBettingRound();

    // Turn
    qDebug() << "Burn card before turn:" << deck.takeLast().toString(); // burn card
    m_turn = deck.takeLast();
    qDebug() << "Turn card:" << m_turn.toString();
    if (activePlayers() > 1)
        doBettingRound();

    // River
    qDebug() << "Burn card before river:" << deck.takeLast().toString(); // burn card
    m_river = deck.takeLast();
    qDebug() << "River card:" << m_river.toString();
    if (activePlayers() > 1)
        doBettingRound();

    if (activePlayers() < 2) {
        for (int i=0; i<m_players.size(); i++) {
            if (!m_players[i].hasFolded()) {
                qDebug() << "Player" << i << "has won" << m_pot << "monies.";
                m_players[i].giveMoney(m_pot);
                return;
            }
        }
    }


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

    int called = 0, current = 0, players = activePlayers();
    do {
        current++;
        current %= m_players.size();

        if (!m_players[current].hasFolded()) {
            action = m_players[current].assess(this);
            switch(action) {
            case (Player::Fold):
                qDebug() << "Player" << current << "folded.";
                called++;
                break;
            case (Player::Call):
                qDebug() << "Player" << current << "called.";
                m_pot += m_lastBet;
                m_players[current].takeMoney(m_lastBet);
                called++;
                break;
            case (Player::Raise):
                qDebug() << "Player" << current << "raised to" << m_players[current].bet() << ".";
                m_pot += m_players[current].bet();
                m_lastBet = m_players[current].bet();
                called = 0; // Everyone now needs to call or fold to this raise
                m_players[current].takeMoney(m_players[current].bet());
            }
        }
        qDebug() << "Active players:" << activePlayers();
        if (activePlayers() < 2)
            return;
    } while (called < players); // make sure all people have called (or folded) since last raise
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
