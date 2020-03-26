#include <iostream>
#include "game.h"

// TODO menu
// TODO bot
// TODO win/lose/draw

int main() {
    Game game = Game();
    CmdHandle handle = CmdHandle();

    while(!game.IsOver()) {
        game.PlayTurn(handle);
    }
}
