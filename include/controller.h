#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "gameboard.h"
#include "player.h"

const int SURRENDER = -2;
const int CLAIM_ENDLESS_CYCLE = -1;

struct Controller {
    Player player;
    Controller(Player p): player(p) {}
    
    virtual int ChoosePit(Gameboard &board) = 0;
    virtual bool ask_endless_cycle() = 0;
};

struct CmdController: public Controller {
    CmdController(Player p);
    
    int ChoosePit(Gameboard &board);
    bool ask_endless_cycle();
};
struct BotController: public Controller {
    BotController(Player p);

    int ChoosePit(Gameboard &board);
    bool ask_endless_cycle();
};

#endif