#ifndef CMD_HANDLE_H
#define CMD_HANDLE_H

#include "cmd_utils.h"
#include "gameboard.h"
#include "range.h"
#include "game.h"

const char TITLE_SCREEN[] = "   ___                         \n"
                            "  / _ \\__      ____ _ _ __ ___ \n"
                            " | | | \\ \\ /\\ / / _` | '__/ _ \\\n"
                            " | |_| |\\ V  V / (_| | | |  __/\n"
                            "  \\___/  \\_/\\_/ \\__,_|_|  \\___|\n";

void DrawMenu();

enum MenuOption {
    PlayerVsPlayer = 1,
    PlayerVsAi = 2,
    ExitGame = 3,
};

MenuOption MenuPrompt();

void DrawGameoverScreen(Game &game);

void DrawGameboard(Gameboard &board);
void SetPit(int pit, int value);
void HighlightCapture(Range capture, Gameboard &board);

#endif