#include "game.h"
#include <iostream>

Game::Game(): current_player(PlayerOne), board(Gameboard()), win_state(OngoingGame) {}

bool Game::IsOver() {
    return board.p1_score >= 25 || board.p2_score >= 25 || (board.p1_score == 24 && board.p2_score == 24);
}

void Game::PlayTurn(CmdHandle &handle) {
    if(board.HasLegalMove(current_player)) {
        int choice = handle.ChoosePit(current_player, board);

        if(choice == WITHDRAW) {
            win_state = current_player == PlayerOne? PlayerTwoWins : PlayerOneWins;
        } else if(choice == CLAIM_ENDLESS_CYCLE) {
            bool opponent_claim = handle.ask_endless_cycle(Opponent(current_player));
            if(opponent_claim) {
                board.Capture(current_player, PlayerBoard(current_player));
                board.Capture(Opponent(current_player), PlayerBoard(Opponent(current_player)));
            }
        } else { 
            int last_sowed = board.Sow(choice, handle);

            if(board.IsCapturable(current_player, last_sowed)) {
                Range capture = board.CaptureRange(current_player, last_sowed);
                // "Grand Slams" cancel capture in abapa version
                if(!board.IsGrandSlam(current_player, capture)) {
                    handle.HighlightCapture(capture, board);
                    board.Capture(current_player, capture);
                }
            }

            current_player = Opponent(current_player);
        }
    } else {
        Range capture = PlayerBoard(current_player);
        handle.HighlightCapture(capture, board);
        board.Capture(current_player, capture);
    }
}
