#include "preflopplayer.h"
#include "table.h"
#include <math.h>

RolloutPlayer::RolloutPlayer(bool offensive) : Player(offensive)
{
    static int count = 0;
    m_preflop.loadData();
    if (!m_preflop.isValid()) {
        m_preflop.generateData(100);
        m_preflop.saveData();
    }
    count++;
    if (offensive)
        m_name = "trolloutplayer" + QString::number(count);
    else
        m_name = "rolloutplayer" + QString::number(count);
}

Player::Action RolloutPlayer::assess(Table *table)
{
    if (table->river().isNull() || table->turn().isNull()) {
        return Player::assess(table);
    }

    int ps;

    if (table->flop().isEmpty()) {
        ps = m_preflop.getProbability(m_cards.first(), m_cards.last(), table->numPlayers()) > 400;
    } else {
        Deck communityCards;
        communityCards << table->flop();
        if (!table->river().isNull())
            communityCards << table->river();
        if (!table->turn().isNull())
            communityCards << table->turn();
        ps = h(m_cards, communityCards, table->activePlayers());
    }

    if (ps > 600 || (ps > 500 && m_offensive)) {
        m_bet = table->lastBet() + 50;
        return Player::Raise;
    } else if (ps >= 500 || (ps > 400 && m_offensive)) {
        return Player::Call;
    } else {
        m_hasFolded = true;
        return Player::Fold;
    }
}

// THEY SEE ME ROLLIN', THEY HATIN'
int RolloutPlayer::h(Deck hand, const Deck &onTable, int players)
{
    hand.append(onTable);
    Deck deck;
    deck.generate();
    deck.removeCards(hand);
    deck.removeCards(onTable);
    deck.shuffle();

    Deck opponent;
    int wins = 0, ties = 0;
    int count = 0;
    for (int x=0; x<deck.size(); x++) {
        for (int y=x; y<deck.size(); y++) {
            count++;
            opponent << deck.takeAt(x)
                     << deck.takeAt(y);
            opponent.append(onTable);
            if (opponent < hand) {
                wins++;
            } else if (!(hand < opponent)) {
                ties++;
            }
        }
    }

    ties *= 1000;
    wins *= 1000;

    wins += ties/2;
    wins /= count;

    wins = pow(wins, players);

    return wins;
}
