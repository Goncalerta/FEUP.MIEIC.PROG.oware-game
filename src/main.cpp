#include "game.h"
#include "cmd_handle.h"
#include "controller.h"

int main() {
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
