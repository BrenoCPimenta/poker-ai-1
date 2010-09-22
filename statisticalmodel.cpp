#include "statisticalmodel.h"

StatisticalModel::StatisticalModel()
{
}

void StatisticalModel::addContext(float potOdds, Player::Action lastAction, const Deck &communityCards)
{
    m_contexts.append(Context(potOdds, lastAction, communityCards));
}
