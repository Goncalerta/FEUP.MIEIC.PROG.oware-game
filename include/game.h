#ifndef GAME_H
#define GAME_H

struct Game;

#include "gameboard.h"
#include "player.h"
#include "cmd_handle.h"

struct Game {
    Gameboard board;
    Player current_player;

    Game();

    void PlayTurn(CmdHandle &handle);
    bool IsOver();
};

#endif