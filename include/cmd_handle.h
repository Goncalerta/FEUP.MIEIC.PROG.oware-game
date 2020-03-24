#ifndef CMD_HANDLE_H
#define CMD_HANDLE_H

struct CmdHandle;
int CharToPit(char c);

#include "gameboard.h"

struct CmdHandle {
    CmdHandle();
    
    void Draw(Gameboard &board);
    void SetPit(int pit, int value);
    int ChoosePit(Player p, Gameboard &board);
};

#endif