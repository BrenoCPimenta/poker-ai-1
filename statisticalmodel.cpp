#include "statisticalmodel.h"

#include <QStringList>
#include <QFile>
#include <QTextStream>

StatisticalModel::StatisticalModel()
{
    load();
}

StatisticalModel::~StatisticalModel()
{
    save();
}

void StatisticalModel::addContext(float potOdds, Player::Action lastAction, const Deck &communityCards, int handPower)
{
    m_contexts.append(Context(potOdds, lastAction, communityCards, handPower));
}

QString StatisticalModel::Context::toString() const
{
    QString ret;
    ret += QString::number(m_potOdds);
    ret += ';';
    ret += QString::number(m_lastAction);
    ret += ';';
    ret += m_communityCards.toString();
    ret += ';';
    ret += QString::number(m_handPower);
    ret += '\n';

    return ret;
}

StatisticalModel::Context StatisticalModel::Context::fromString(QString string)
{
    QStringList elements = string.split(';');
    float potOdds = elements[0].toFloat();
    Player::Action action = (Player::Action)elements[1].toInt();
    Deck deck = Deck::fromString(elements[2]);
    int handPower = elements[3].toInt();

    return Context(potOdds, action, deck, handPower);
}

void StatisticalModel::save(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    foreach(const Context &context, m_contexts) {
        out << context.toString();
    }
}

void StatisticalModel::load(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    while (!file.atEnd()) {
        m_contexts.append(Context::fromString(file.readLine()));
    }
}
