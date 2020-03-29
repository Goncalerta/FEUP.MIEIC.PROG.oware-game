#include "game.h"
#include "cmd_handle.h"

Game::Game(): current_player(PlayerOne), board(Gameboard()), win_state(OngoingGame), surrendered(false) {}

void Game::UpdateWinState() {
    if(board.p1_score >= 25) win_state = PlayerOneWins;
    else if(board.p2_score >= 25) win_state = PlayerTwoWins;
    else if(board.p1_score == 24 && board.p2_score == 24) win_state = Draw;
}

void Game::PlayTurn(Controller *p1_controller, Controller *p2_controller) {
    Controller *player_controller = (current_player == PlayerOne? p1_controller : p2_controller);
    Controller *opponent_controller = (current_player == PlayerOne? p2_controller : p1_controller);

    if(board.HasLegalMove(current_player)) {
        int choice = player_controller->ChoosePit(board);

        if(choice == SURRENDER) {
            surrendered = true;
            win_state = current_player == PlayerOne? PlayerTwoWins : PlayerOneWins;
        } else if(choice == CLAIM_ENDLESS_CYCLE) {
            bool opponent_claim = opponent_controller->ask_endless_cycle();
            if(opponent_claim) {
                HighlightCapture(PlayerBoard(current_player), current_player, board);
                board.Capture(current_player, PlayerBoard(current_player));
                HighlightCapture(PlayerBoard(Opponent(current_player)), Opponent(current_player), board);
                board.Capture(Opponent(current_player), PlayerBoard(Opponent(current_player)));
                UpdateWinState();
            }

        } else { 
            int last_sowed = board.Sow(choice, SetPit);

            if(board.IsCapturable(current_player, last_sowed)) {
                Range capture = board.CaptureRange(current_player, last_sowed);
                // "Grand Slams" cancel capture in abapa version
                if(!board.IsGrandSlam(current_player, capture)) {
                    HighlightCapture(capture, current_player, board);
                    board.Capture(current_player, capture);
                    UpdateWinState();
                }
            }

            current_player = Opponent(current_player);
        }
    } else {
        Range capture = PlayerBoard(current_player);
        HighlightCapture(capture, current_player, board);
        board.Capture(current_player, capture);
        UpdateWinState();
    }
}
