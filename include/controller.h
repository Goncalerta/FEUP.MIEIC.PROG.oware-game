#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "gameboard.h"
#include "player.h"

// Number of milliseconds the bot waits before returning its
// calculated move. 
const int BOT_THINKING_DELAY = 800;

// The return value of `Controller::ChoosePit` when the choice
// is to surrender.
const int SURRENDER = -2;
// The return value of `Controller::ChoosePit` when the choice
// is to claim that the game has been reduced to an endless cycle.
const int CLAIM_ENDLESS_CYCLE = -1;

// The bridge between the game and the player.
//
// This interface is used for the game to get input from the player,
// which could be an user or a bot.
struct Controller {
    // The player being controlled.
    Player player;
    Controller(Player p): player(p) {}
    
    // Asks to make a move in the given `Gameboard`.
    // 
    // It must either return a pit that is valid (number between 0 and 11)
    // and sowable (`Gameboard::Sowable(current_player, pit)` must return `ValidPit`),
    // or the special values `SURRENDER` or `CLAIM_ENDLESS_CYCLE`.
    virtual int ChoosePit(Gameboard &board) = 0;

    // Asks whether or not the game has been reduced to an endless cycle.
    // 
    // This method is called after the opponent has claimed that the game
    // has been reduced to an endless cycle, so as to make sure that both
    // players are in the same page.
    virtual bool ask_endless_cycle() = 0;
};

// The interface between the `Game` and the user in the terminal console.
//
// Responsible for printing information on the console and prompting the
// user to respond.
struct CmdController: public Controller {
    CmdController(Player p);
    
    int ChoosePit(Gameboard &board);
    bool ask_endless_cycle();
};

// The interface between the `Game` and the bot for 'Player Vs. AI'.
// 
// Responsible for responding automaticlaly and printing information on 
// the console about the move that the bot is making.
struct BotController: public Controller {
    BotController(Player p);

    int ChoosePit(Gameboard &board);
    bool ask_endless_cycle();
};

#endif
