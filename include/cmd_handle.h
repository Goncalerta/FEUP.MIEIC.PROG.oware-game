#ifndef CMD_HANDLE_H
#define CMD_HANDLE_H

struct CmdHandle;
int CharToPit(char c);

#include "gameboard.h"
#include "range.h"

const int WITHDRAW = -2;
const int CLAIM_ENDLESS_CYCLE = -1;

struct CmdHandle {
    CmdHandle();
    
    void Draw(Gameboard &board);
    void SetPit(int pit, int value);
    int ChoosePit(Player p, Gameboard &board);
    void HighlightCapture(Range capture, Gameboard &board);
    bool ask_endless_cycle(Player p);
};

#endif