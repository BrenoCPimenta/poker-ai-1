#include "context.h"

Context::Context(float potOdds, Player::Action lastAction, const Deck &communityCards) :
        m_potOdds(potOdds),
        m_lastAction(lastAction),
        m_communityCards(communityCards)
{
}
