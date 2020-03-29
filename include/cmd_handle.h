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

const int TITLE_SCREEN_COLOR = WHITE;
const int TEXT_COLOR = LIGHTGRAY;
const int MENU_NUMBER_COLOR = DARKGRAY;
const int PLAYER_ONE_COLOR = LIGHTBLUE;
const int PLAYER_TWO_COLOR = LIGHTRED;
const int PLAYER_NEUTRAL_COLOR = YELLOW;
const int PLAYER_ONE_SOWABLE_COLOR = LIGHTBLUE;
const int PLAYER_ONE_UNSOWABLE_COLOR = BLUE;
const int PLAYER_TWO_SOWABLE_COLOR = LIGHTRED;
const int PLAYER_TWO_UNSOWABLE_COLOR = RED;

const int GAMEOVER_SCREEN_MILLISECONDS = 6000;

const int GAME_PROMPT_LINE = 8;

int CharToPit(char c);
char PitToChar(int pit);

void DrawPlayerLabel(Player p);

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
void HighlightCapture(Range capture, Player p, Gameboard &board);

#endif