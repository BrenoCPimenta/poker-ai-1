#include "preflopplayer.h"
#include "table.h"

PreFlopPlayer::PreFlopPlayer()
{
    m_preflop.loadData();
    if (!m_preflop.isValid()) {
        m_preflop.generateData(100);
        m_preflop.saveData();
    }
}

Player::Action PreFlopPlayer::assess(Table *table)
{
    if (!table->flop().isEmpty()) {
        return Player::assess(table);
    }
    int ps= m_preflop.getProbability(m_cards.first(), m_cards.last(), table->numPlayers()) > 400;
    if (ps > 600) {
        m_bet = table->lastBet() + 50;
        return Player::Raise;
    } else if (ps >= 500) {
        return Player::Call;
    } else {
        m_hasFolded = true;
        return Player::Fold;
    }
}
