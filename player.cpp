#include "player.h"

Player::Player() :
    m_hasFolded(false),
    m_money(1000),
    m_bet(0)
{
}

Player::Action Player::assess(Table *table)
{
    if (table->flop().isEmpty()) {
        return Call;
    }

    if (handStrength(table) < 5) {
        m_hasFolded = true;
        return Fold;
    } else if (lastHandStrength() > 5 && lastHandStrength() < 8)
        return Call;
    else {
        m_bet = table->lastBet() + 50;
        return Raise;
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