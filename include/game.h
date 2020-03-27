#ifndef GAME_H
#define GAME_H

struct Game;
enum WinState {
    OngoingGame,
    PlayerOneWins,
    PlayerTwoWins,
    Draw,
};

#include "gameboard.h"
#include "player.h"
#include "cmd_handle.h"

struct Game {
    Gameboard board;
    Player current_player;
    WinState win_state;

    Game();

    void PlayTurn(CmdHandle &handle);
    void UpdateWinState();
};

#endif