#ifndef PLAYER_H
#define PLAYER_H

#include "range.h"

enum Player{
    PlayerOne, PlayerTwo
};

Player PitZone(int pit);
Player Opponent(Player p);
Range PlayerBoard(Player p);

#endif