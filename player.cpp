#include "player.h"
#include "table.h"

#include <QDebug>

Player::Player(bool offensive) :
    m_hasFolded(false),
    m_money(1000),
    m_bet(0),
    m_offensive(offensive),
    m_win(0),
    m_tie(0)
{
    static int count=0;
    count++;

    if (offensive) {
        m_name = "offensive" + QString::number(count);
    } else {
        m_name = "conservative" + QString::number(count);
    }

}

Player::Action Player::assess(Table *table)
{
    if (table->flop().isEmpty() && qrand() % 2 == 1)
        return Fold;

    if (m_offensive) {
        if (table->lastBet() < 150) {
            m_bet = 50;
            return Raise;
        }

        if (table->flop().isEmpty()) {
            return Call;
        }

        if (handStrength(table) < 4) {
            m_hasFolded = true;
            return Fold;
        } else if (lastHandStrength() > 6 && lastHandStrength() < 7)
            return Call;
        else {
            m_bet = table->lastBet() + 50;
            return Raise;
        }
    } else {
        if (table->lastBet() < 150) {
            m_bet = 50;
            return Raise;
        }

        if (table->flop().isEmpty()) {
            return Call;
        }

        if (handStrength(table) < 5) {
            m_hasFolded = true;
            return Fold;
        } else if (lastHandStrength() > 7 && lastHandStrength() < 8)
            return Call;
        else {
            m_bet = table->lastBet() + 50;
            return Raise;
        }
    }
}

int Player::handStrength(Table *table)
{
    Deck cards(m_cards);
    cards.append(table->flop());
    if (!table->turn().isNull())
        cards.append(table->turn());
    if (!table->river().isNull())
        cards.append(table->river());

    m_lastHandStrength = cards.strength();
    return m_lastHandStrength;
}

void Player::setDeck(Deck deck)
{
    m_cards = deck;
    m_hasFolded = false;
    m_cards.printOut();
}

