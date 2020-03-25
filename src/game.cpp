#include "game.h"
#include <iostream>

Game::Game(): current_player(PlayerOne), board(Gameboard()) {}

bool Game::IsOver() {
    return board.p1_score >= 25 || board.p2_score >= 25 || (board.p1_score == 24 && board.p2_score == 24);
}


void Game::PlayTurn(CmdHandle &handle) {
    int chosen_pit = handle.ChoosePit(current_player, board);
    int last_sowed = board.Sow(chosen_pit, handle);

    if(board.IsCapturable(current_player, last_sowed)) {
        Range capture = board.CaptureRange(current_player, last_sowed);
        // "Grand Slams" cancel capture in abapa version
        if(!board.IsGrandSlam(current_player, capture)) {
            board.Capture(current_player, capture);
        }
    }

    current_player = Opponent(current_player);
}
