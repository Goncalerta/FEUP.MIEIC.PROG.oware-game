#include <iostream>
#include "game.h"

int main() {
    Game game = Game();
    CmdHandle handle = CmdHandle();

    while(!game.IsOver()) {
        game.PlayTurn(handle);
        
    }
}
