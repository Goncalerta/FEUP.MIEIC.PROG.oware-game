#include <iostream>
#include "cmd_utils.h"
#include "game.h"

int CharToPit(char c) {
    switch(c) {
        case 'a': return 0;
        case 'b': return 1;
        case 'c': return 2;
        case 'd': return 3;
        case 'e': return 4;
        case 'f': return 5;
        case 'A': return 6;
        case 'B': return 7;
        case 'C': return 8;
        case 'D': return 9;
        case 'E': return 10;
        case 'F': return 11;
        default: return -1;
    }
}

int main() {
    Game game = Game();
    char user_input;

    while(true) {
        clrscr();
        game.board.Draw();
        std::cout << std::endl << std::endl;
        bool playing_p1 = game.current_player == PlayerOne;
        std::cout << "Player " << (playing_p1? 1 : 0) << " is playing this turn." << std::endl;
        std::cout << "Input pit letter (" << (playing_p1? "a-f" : "A-F") << "): ";
        // TODO improve error handling for invalid inputs (ex. multiple inputs separated by space, non chars etc)
        std::cin >> user_input;
        int chosen_pit = CharToPit(user_input);
        if (chosen_pit != -1 && game.board.IsSowable(game.current_player, chosen_pit)) {
            CaptureRange capture = game.board.Sow(chosen_pit);
            if(capture.IsValid() && game.board.IsCapturable(game.current_player, capture)) {
                game.board.Capture(game.current_player, capture);
            }

            game.current_player = Opponent(game.current_player);
        } else {
            // TODO error message
        }
    }
}
