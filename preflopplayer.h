#ifndef PREFLOPPLAYER_H
#define PREFLOPPLAYER_H

#include "player.h"
#include "preflop.h"

class PreFlopPlayer : public Player
{
public:
    PreFlopPlayer();

    virtual Player::Action assess(Table *table);

    PreFlop m_preflop;
};

#endif // PREFLOPPLAYER_H
