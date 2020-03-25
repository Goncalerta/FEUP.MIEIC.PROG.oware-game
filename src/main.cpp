#include <iostream>
#include "game.h"

// TODO player give up
// TODO agree endless cycle
// TODO menu
// TODO bot

int main() {
    Game game = Game();
    CmdHandle handle = CmdHandle();

    while(!game.IsOver()) {
        game.PlayTurn(handle);
        
    }
}
