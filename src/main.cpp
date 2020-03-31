#include "game.h"
#include "cmd_handle.h"
#include "controller.h"
#include <cstdlib>  
#include <ctime> 

int main() {
    // RNG will be used by bots to choose between moves with
    // very similar immediate payoff.
    srand(time(0));

    Game game;
    Controller *p1_controller, *p2_controller;

    while(true) {
        DisplayMenu();

        switch(MenuPrompt()) {
            case PlayerVsPlayer:
                game = Game();
                p1_controller = new CmdController(PlayerOne);
                p2_controller = new CmdController(PlayerTwo);
                break;

            case PlayerVsAi:
                game = Game();
                p1_controller = new CmdController(PlayerOne);
                p2_controller = new BotController(PlayerTwo);
                break;
                
            case ExitGame:
                return 0;
        }

        while(game.win_state == OngoingGame) {
            game.PlayTurn(p1_controller, p2_controller);
        }

        DisplayGameoverScreen(game);
    }
}
