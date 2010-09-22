#include "table.h"
#include "player.h"
#include "preflopplayer.h"

#define MAX_BETTINGROUNDS 5

#include <QDebug>

Table::Table(QObject *parent) :
        QObject(parent),
        m_pot(0),
        m_lastBet(0)
{
    m_players << Player(true)
              << Player(false)
              << Player(false)
              << RolloutPlayer();
}

void Table::play(int rounds)
{
    for (int round=0; round<rounds; round++) {
        playHand();
    }

    qDebug() << "Finished playing...";
    for (int i=0; i<m_players.size(); i++) {
        qWarning() << m_players[i].name() << "won"
                << m_players[i].wins()
                << "times and tied"
                << m_players[i].ties() << "times.";
    }
}

void Table::playHand()
{
    m_players.append(m_players.takeFirst()); //rotate players playing first

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
        qDebug() << "Giving player" << m_players[i].name() << "a deck...";
        m_players[i].setDeck(deck.take(2));
        qDebug() << "Player" << m_players[i].name() << "has" << m_players[i].money() << "monies.";
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

    // Check if all but one has folded
    if (activePlayers() < 2) {
        for (int i=0; i<m_players.size(); i++) {
            if (!m_players[i].hasFolded()) {
                qDebug() << "Player" << m_players[i].name() << "has won" << m_pot << "monies.";
                m_players[i].win();
                m_players[i].giveMoney(m_pot);
                return;
            }
        }
    }


    int strongestHand = -1;

    QList<int> winners;
    for (int i=0; i<m_players.size(); i++)
        winners << i;
    for (int i=0; i<m_players.size(); i++)
        if (m_players[i].hasFolded())
            winners.removeAll(i);

    Deck d, o;
    // Stupid, late, slow, not thinking
    for (int i=0; i<m_players.size(); i++) {
        for (int j=0; j<m_players.size(); j++) {
            if (j==i) continue;
            d = m_players[i].deck();
            d << m_flop
                 << m_river
                 << m_turn;
            o = m_players[j].deck();
            o << m_flop
                  << m_river
                  << m_turn;
            if (d < o)
                winners.removeAll(i);
        }
    }
    qDebug() << "We have" << winners.size() << "winners.";
    if (winners.size() > 1) {
        int potPayout = m_pot / winners.size();
        foreach(int i, winners) {
            m_players[i].tie();
            qDebug() << "Player" << m_players[i].name() << "has tied and gotten" << potPayout << "casharoos.";
            m_players[i].giveMoney(potPayout);
        }
    }
    else {
        qDebug() << "Player" << m_players[winners[0]].name() << "has won" << m_pot;
        m_players[winners[0]].win();
        m_players[winners[0]].giveMoney(m_pot);
    }
}

void Table::doBettingRound()
{
    Player::Action action;

    for (int i=0; i<m_players.size(); i++) {
        m_players[i].bets = 0;
    }

    int called = 0, current = 0, players = activePlayers();
    do {
        current++;
        current %= m_players.size();

        if (!m_players[current].hasFolded()) {
            action = m_players[current].assess(this);
            m_players[current].bets++;
            if (m_players[current].bets > MAX_BETTINGROUNDS)
                action = Player::Call;

            switch(action) {
            case (Player::Fold):
                qDebug() << "Player" << m_players[current].name() << "folded.";
                called++;
                break;
            case (Player::Call):
                qDebug() << "Player" << m_players[current].name() << "called.";
                m_pot += m_lastBet;
                m_players[current].takeMoney(m_lastBet);
                called++;
                break;
            case (Player::Raise):

                qDebug() << "Player" << m_players[current].name() << "raised to" << m_players[current].bet() << ".";
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
