#include "game.h"
#include "range.h"
#include "cmd_handle.h"

Game::Game(): current_player(PlayerOne), board(Gameboard()), win_state(OngoingGame), surrendered(false) {}

void Game::UpdateWinState() {
    if(board.p1_score >= 25) win_state = PlayerOneWins;
    else if(board.p2_score >= 25) win_state = PlayerTwoWins;
    else if(board.p1_score == 24 && board.p2_score == 24) win_state = Draw;
}

void Game::PlayTurn(Controller *p1_controller, Controller *p2_controller) {
    Controller *player_controller, *opponent_controller;
    
    if(current_player == PlayerOne) {
        player_controller = p1_controller;
        opponent_controller = p2_controller;
    } else {
        player_controller = p2_controller;
        opponent_controller = p1_controller;
    }

    if(board.HasLegalMove(current_player)) {
        int choice = player_controller->ChoosePit(board);

        if(choice == SURRENDER) {
            Surrender(current_player);

        } else if(choice == CLAIM_ENDLESS_CYCLE) {
            bool opponent_claim = opponent_controller->ask_endless_cycle();
            if(opponent_claim) EndlessCycle();
        
        } else { 
            PlayMove(choice);
            current_player = Opponent(current_player);
        }
    } else {
        OutOfMoves();
    }
}

void Game::PlayMove(int choice) {
    int last_sowed = board.Sow(choice, SetPit);

    if(board.IsCapturable(current_player, last_sowed)) {
        Range capture = board.CaptureRange(current_player, last_sowed);
        
        // "Grand Slams" cancel capture in abapa version
        if(!board.IsGrandSlam(current_player, capture)) {
            board.Capture(current_player, capture, SetScoreAnimation, HighlightCapture);
            UpdateWinState();
        }
    }
}

void Game::OutOfMoves() {
    DisplayOutOfMovesGameboard(board, current_player);
    
    Range capture = PlayerZone(current_player);
    board.Capture(current_player, capture, SetScoreAnimation, HighlightCapture);
    
    UpdateWinState();
}

void Game::EndlessCycle() {
    Player opponent = Opponent(current_player);
    Range player_zone = PlayerZone(current_player);
    Range opponent_zone = PlayerZone(opponent);
    
    board.Capture(current_player, player_zone, SetScoreAnimation, HighlightCapture);
    board.Capture(opponent, opponent_zone, SetScoreAnimation, HighlightCapture);
    
    UpdateWinState();
}

void Game::Surrender(Player p) {
    surrendered = true;
    win_state = (p == PlayerOne? PlayerTwoWins : PlayerOneWins);
}
