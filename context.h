#ifndef CONTEXT_H
#define CONTEXT_H

#include "player.h"

class Context
{
public:
    Context(float potOdds, Player::Action lastAction, const Deck &communityCards);

private:
    float m_potOdds;
    Player::Action m_lastAction;
    Deck m_communityCards;
};

#endif // CONTEXT_H
