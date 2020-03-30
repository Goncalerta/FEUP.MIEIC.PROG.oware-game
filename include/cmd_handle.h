#ifndef CMD_HANDLE_H
#define CMD_HANDLE_H

#include "cmd_utils.h"
#include "gameboard.h"
#include "range.h"
#include "game.h"
#include <string>

// The string that contains the title screen of the game.
const char TITLE_SCREEN[] = "   ___                         \n"
                            "  / _ \\__      ____ _ _ __ ___ \n"
                            " | | | \\ \\ /\\ / / _` | '__/ _ \\\n"
                            " | |_| |\\ V  V / (_| | | |  __/\n"
                            "  \\___/  \\_/\\_/ \\__,_|_|  \\___|\n";


// The color that the title screen should be printed in.
const int TITLE_SCREEN_COLOR = WHITE;
// The color for regular text.
const int TEXT_COLOR = LIGHTGRAY;
// The color to represent menu options' index.
const int MENU_NUMBER_COLOR = DARKGRAY;
// The main color related to Player 1.
const int PLAYER_ONE_COLOR = LIGHTBLUE;
// The main color related to Player 1.
const int PLAYER_TWO_COLOR = LIGHTRED;
// A color neutral to both players (for example, for draws).
const int PLAYER_NEUTRAL_COLOR = YELLOW;
// The color to represent Player 1's sowable pits.
const int PLAYER_ONE_SOWABLE_COLOR = LIGHTBLUE;
// The color to represent Player 1's unsowable pits.
const int PLAYER_ONE_UNSOWABLE_COLOR = BLUE;
// The color to represent Player 2's sowable pits.
const int PLAYER_TWO_SOWABLE_COLOR = LIGHTRED;
// The color to represent Player 2's unsowable pits.
const int PLAYER_TWO_UNSOWABLE_COLOR = RED;
// The color of score when being updated.
const int SET_SCORE_COLOR = LIGHTGREEN;


// The number of milliseconds the gameover screen lasts on
// screen before returning to main menu.
const int GAMEOVER_SCREEN_MILLISECONDS = 6000;
// The number of milliseconds between each pit being set
// while sowing.
const int UPDATE_PIT_DELAY = 200;
// The number of milliseconds between each tick while
// score is being updated.
const int UPDATE_SCORE_TICK = 100;
// The number of milliseconds after the score has been
// updated, before resuming the game.
const int SCORE_UPDATED_DELAY = 1000;
// The number of milliseconds after a capture has been
// highlighted, before resuming the game.
const int CAPTURE_DELAY = 600;


// The console line where Player 2's pits are printed.
const int PLAYER_TWO_PITS_LINE = 1;
// The console line where Player 1's pits are printed.
const int PLAYER_ONE_PITS_LINE = 3;
// The console column where leftmost pits are printed.
const int PITS_START_COLUMN = 14;
// The console line where it is printed who is playing
// the current turn.
const int GAME_PROMPT_LINE = 8;
// The console column where the score is printed.
const int PLAYER_SCORE_COLUMN = 48;


// Converts a given `char c` to the pit it represents.
// If it doesn't represent a valid pit, returns -1.
int CharToPit(char c);

// Returns the `char` represented by the given `pit`.
// If it is not a valid pit, returns ' '.
char PitToChar(int pit);


// Returns the char used by `Player p` to surrender.
char QuitChar(Player p);
// Returns the char used by `Player p` to claim and endless cycle.
char ClaimEndlessCycleChar(Player p);
// Returns the char used by `Player p` to agree to a 'Y/N' prompt.
char AgreeChar(Player p);
// Returns the char used by `Player p` to disagree to a 'Y/N' prompt.
char DisagreeChar(Player p);


// Prints the label of the given `Player p` in its main color.
void DrawPlayerLabel(Player p);


// Prints the title screen in its color.
void DrawTitleScreen();

// Prints a menu option indexed by `number` labeled as `option`. 
void DrawMenuOption(int number, std::string option);

// Clears the console and shows the main menu.
void DisplayMenu();

// The return type of `MenuPrompt()`. Represents the option
// chosen by the player.
enum MenuOption {
    PlayerVsPlayer = 1,
    PlayerVsAi = 2,
    ExitGame = 3,
};

// Prompts the player to choose an option from the main menu.
//
// Returns the player's choice.
MenuOption MenuPrompt();


// Clears the console and shows the gameover screen of the given `Game`.
void DisplayGameoverScreen(Game &game);


// Prints the given `Gameboard`. 
//
// If `p1_controls` is true, show Player 1's 'surrender' and 'claim endless cycle' characters.
// If `p2_controls` is true, show Player 2's 'surrender' and 'claim endless cycle' characters.
void DrawGameboard(Gameboard &board, bool p1_controls, bool p2_controls);

// Clears the console and shows the given `Gameboard`, printing that `Player p` is out of legal
// moves.
void DisplayOutOfMovesGameboard(Gameboard &board, Player p);

// Clears the console and shows the given `Gameboard`, prompting `Player p` to make a move.
// 
// If `warning_message` is not an empty string, it will also be printed. This message allows
// the prompt to comunicate that previous input from the user was invalid (and the reason).
void DisplayPlayingGameboard(Gameboard &board, Player p, std::string warning_message);

// Clears the console and shows the given `Gameboard`, printing that `Player p` is doing the
// next move. Used to show the user the move made by a bot.
void DisplayBotPlayingGameboard(Gameboard &board, Player p);


// Sets the console's cursor on the coordinates where the given `pit` is printed.
void SetCursorOnPit(int pit);

// Updates the given `pit` with `value`.
void SetPit(int pit, int value);

// Updates `Player p`'s score with `value`.
//
// This function is intended to be used as a `SetPitAnimator`.
void SetScore(Player p, int value);

// Shows the score of `Player p` being updated.
//
// This function is intended to be used as a `SetScoreAnimator`.
void SetScoreAnimation(Player p, int old_value, int new_value);

// Highlights the `Range capture` as being captured by `Player p`.
//
// This function is intended to be used as a `CaptureAnimator`.
void HighlightCapture(Player p, Range capture, int *pits);


// Prompts `Player p` with a 'Y/N' question and returns whether the
// player agreed to it.
bool PromptYesNo(Player p);

#endif
