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
#include "controller.h"

struct Game {
    Gameboard board;
    Player current_player;
    WinState win_state;
    bool surrendered;

    Game();

    void PlayTurn(Controller *p1_controller, Controller *p2_controller);
    void UpdateWinState();
};

#endif