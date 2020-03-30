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
const int SET_SCORE_COLOR = LIGHTGREEN;

const int GAMEOVER_SCREEN_MILLISECONDS = 6000;

const int PLAYER_TWO_PITS_LINE = 1;
const int PLAYER_ONE_PITS_LINE = 3;
const int GAME_PROMPT_LINE = 8;
const int PLAYER_SCORE_COLUMN = 48;

int CharToPit(char c);
char PitToChar(int pit);

char QuitChar(Player p);
char ClaimEndlessCycleChar(Player p);

void DrawPlayerLabel(Player p);

void DisplayMenu();

enum MenuOption {
    PlayerVsPlayer = 1,
    PlayerVsAi = 2,
    ExitGame = 3,
};

MenuOption MenuPrompt();

void DisplayGameoverScreen(Game &game);

void DrawGameboard(Gameboard &board, bool p1_controls, bool p2_controls);
void ShowOutOfMovesGameboard(Gameboard &board, Player p);
void SetPit(int pit, int value);
void SetScoreAnimation(Player p, int old_value, int new_value);
void HighlightCapture(Player p, Range capture, int *pits);

#endif
