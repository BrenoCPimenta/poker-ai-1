#include "preflopplayer.h"
#include "table.h"
#include <math.h>

#include <QDebug>

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


    if (table->flop().isEmpty()) {
        int ps = m_preflop.getProbability(m_cards.first(), m_cards.last(), table->numPlayers()) > 400;
        if (ps > 600 || (ps > 500 && m_offensive)) {
            m_bet = table->lastBet() + 50;
            return Player::Raise;
        } else if (ps >= 400 || (ps > 300 && m_offensive)) {
            return Player::Call;
        } else {
            m_hasFolded = true;
            return Player::Fold;
        }

    } else {
        Deck communityCards;
        communityCards << table->flop();
        if (!table->river().isNull())
            communityCards << table->river();
        if (!table->turn().isNull())
            communityCards << table->turn();

        int ps = h(m_cards, communityCards, table->activePlayers());
        int potOdds = table->lastBet()*1000 / (table->lastBet() + table->pot());
        if (potOdds > ps) {
            if (1.5*ps > potOdds || m_offensive) {
                m_bet = table->lastBet() + 50;
                return Player::Raise;
            } else if (ps > 500)
                return Player::Call;
            else {
                m_hasFolded = true;
                return Player::Fold;
            }
        }
    }

}

// THEY SEE ME ROLLIN', THEY HATIN'
int RolloutPlayer::h(Deck hand, const Deck &onTable, int players)
{
    hand.append(onTable);

    Deck deck;
    deck.generate();
    deck.removeCards(hand);
//    deck.removeCards(onTable);

    Deck opponent;
    float wins = 0, ties = 0, count = 0;
    for (int x=0; x<deck.size(); x++) {
        for (int y=x+1; y<deck.size(); y++) {
            count++;
            opponent.clear();
            opponent << deck.at(x)
                     << deck.at(y);
            opponent.append(onTable);
            if (opponent < hand) {
                wins++;
            } else if (!(hand < opponent)) {
                ties++;
            }
        }
    }

    /*ties *= 1000.0f;
    wins *= 1000.0f;*/

    /*qDebug() << "wins:" << wins;
    qDebug() << "ties:" << ties;
    qDebug() << "count:" << count;*/

    wins += ties/2.0f;
    wins /= count;
    wins = pow(wins, players);
    return (int)(wins*1000);
}
