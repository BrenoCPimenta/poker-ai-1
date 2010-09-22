#ifndef PREFLOPPLAYER_H
#define PREFLOPPLAYER_H

#include "player.h"
#include "preflop.h"

class RolloutPlayer : public Player
{
public:
    RolloutPlayer(bool offensive = false);

    virtual Player::Action assess(Table *table);

private:
    int h(Deck hand, const Deck &onTable, int players);

    PreFlop m_preflop;
};

#endif // PREFLOPPLAYER_H
