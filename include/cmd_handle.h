#ifndef CMD_HANDLE_H
#define CMD_HANDLE_H

struct CmdHandle;
int CharToPit(char c);

#include "gameboard.h"
#include "range.h"

void DrawGame(Gameboard &board);
void SetPit(int pit, int value);
void HighlightCapture(Range capture, Gameboard &board);

#endif