#ifndef STATISTICALMODEL_H
#define STATISTICALMODEL_H

#include <QList>

#include "player.h"

class StatisticalModel
{
    class Context
    {
    public:
        Context(float potOdds, Player::Action lastAction, const Deck &communityCards, int handPower) :
                m_potOdds(potOdds),
                m_lastAction(lastAction),
                m_communityCards(communityCards),
                m_handPower(handPower)
        { }

        QString toString() const;

        static Context fromString(QString string);

        float m_potOdds;
        Player::Action m_lastAction;
        Deck m_communityCards;
        int m_handPower;
    };

public:


    StatisticalModel();
    ~StatisticalModel();

    void addContext(float potOdds, Player::Action lastAction, const Deck &communityCards, int handPower);

    int estimateHandPower(float potOdds, Player::Action lastAction, const Deck &communityCards);

    void save(QString filename = "playermodel.csv");
    void load(QString filename = "playermodel.csv");

private:
    QList<Context> m_contexts;
};

#endif // STATISTICALMODEL_H
