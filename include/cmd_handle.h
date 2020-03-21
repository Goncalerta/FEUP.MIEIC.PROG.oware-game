#ifndef CMD_HANDLE_H
#define CMD_HANDLE_H

struct CmdHandle;
int CharToPit(char c);

#include "gameboard.h"
#include "game.h"

struct CmdHandle {
    CmdHandle();
    
    void Draw(Gameboard &board);
    int ChoosePit(Game &game);
};

#endif