#ifndef GAME_H
#define GAME_H

#include "gameboard.h"
#include "player.h"

typedef struct Game {
    Gameboard board;
    Player current_player;

    Game();
} Game;

#endif