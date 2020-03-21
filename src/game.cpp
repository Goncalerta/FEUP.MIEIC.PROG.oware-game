#include "game.h"
#include <iostream>

Game::Game(): current_player(PlayerOne), board(Gameboard()) {}

bool Game::IsOver() {
    return board.p1_score == 25 || board.p2_score == 25 || (board.p1_score == 24 && board.p2_score == 24);
}


void Game::PlayTurn(CmdHandle &handle) {
    handle.Draw(board);
    int chosen_pit = handle.ChoosePit(*this);
    
    // TODO handle should be the one responsible for returning a valid pit
    if (chosen_pit != -1 && board.IsSowable(current_player, chosen_pit)) {
        CaptureRange capture = board.Sow(chosen_pit);
        if(capture.IsValid() && board.IsCapturable(current_player, capture)) {
            board.Capture(current_player, capture);
        }

        current_player = Opponent(current_player);
    } else {
        // TODO error message
    }
}
