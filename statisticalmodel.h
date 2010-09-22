#ifndef STATISTICALMODEL_H
#define STATISTICALMODEL_H

#include <QList>

#include "context.h"

class StatisticalModel
{
public:
    StatisticalModel();

    void addContext(float potOdds, Player::Action lastAction, const Deck &communityCards);

private:
    QList<Context> m_contexts;
};

#endif // STATISTICALMODEL_H
