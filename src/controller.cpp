#include "controller.h"
#include "cmd_handle.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>  
#include <ctime> 

BotController::BotController(Player p): Controller(p) {}

// TODO [cleanup] this should be possible to join with actual move
Gameboard SimulateMove(Gameboard &board, int pit) {
    Gameboard simulation = board;
    Player current_player = PitOwner(pit);
    
    int last_sowed = simulation.Sow(pit);

    if(simulation.IsCapturable(current_player, last_sowed)) {
        Range capture = simulation.CaptureRange(current_player, last_sowed);

        if(!simulation.IsGrandSlam(current_player, capture)) {
            simulation.Capture(current_player, capture);
        }
    }

    return simulation;
}

// TODO [cleanup] this should be possible to join with actual move
Gameboard SimulateOutOfMoves(Gameboard &board) {
    Gameboard simulation = board;

    simulation.Capture(PlayerOne, PlayerZone(PlayerOne));
    simulation.Capture(PlayerTwo, PlayerZone(PlayerTwo));

    return simulation;
}

// TODO [cleanup] what to do with this?
int BiggestCaptureMoveScore(Gameboard &board, Player p) {
    Range player_pits = PlayerZone(p);
    int score = 0;
    
    if(board.HasLegalMove(p)){
        for(int pit = player_pits.begin; pit <= player_pits.end; pit++) {
            if(board.Sowable(p, pit) != ValidPit) continue;
            
            Gameboard simulation = SimulateMove(board, pit);

            int new_score = simulation.PlayerScore(p);
            if(new_score >= 25) return new_score;

            if(new_score > score) {
                new_score = score;
            }
        }
    } else {
        Gameboard simulation = SimulateOutOfMoves(board);
        score = simulation.PlayerScore(p);
    }

    return score;
}

int BotController::ChoosePit(Gameboard &board) {
    DisplayBotPlayingGameboard(board, player);

    Range player_zone = PlayerZone(player);
    Range opponent_zone = PlayerZone(Opponent(player));

    int old_score = board.PlayerScore(player);
    int opponent_old_score = board.PlayerScore(Opponent(player));

    // The default choice is surrender, however the bot should
    // try to choose any legal move over it. As so, the initial
    // lean is negative, as the calculated lean will always be
    // positive no matter how bad the move is.
    int choice = SURRENDER;
    int choice_lean = -1;
    
    // Simulate legal moves to find out which one gives the biggest
    // immediate payoff.
    for(int pit = player_zone.begin; pit <= player_zone.end; pit++) {
        if(board.Sowable(player, pit) != ValidPit) continue;
        // Any legal move makes more sense that surrendering, even
        // if it is a guaranteed lost.
        if(choice == SURRENDER) choice = pit;

        Gameboard simulation = SimulateMove(board, pit);

        int new_score = simulation.PlayerScore(player);
        if(new_score >= 25) return pit; // Always choose winning moves

        int opponent_new_score = BiggestCaptureMoveScore(simulation, Opponent(player));
        if(opponent_new_score >= 25) continue; // Always avoid losing moves

        // A positive integer that reflects how good this play is in terms
        // of immediate payoff.
        int lean = ((new_score - old_score) - (opponent_new_score - opponent_old_score)) + 25;

        // TODO probability problem
        if(lean > choice_lean || (lean == choice_lean && rand()%2 == 0)) {
            choice_lean = lean;
            choice = pit;
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(BOT_THINKING_DELAY));
    std::cout << PitToChar(choice) << std::endl;
    return choice;
}

bool BotController::ask_endless_cycle() {
    // The bot will just believe the player, as there is no
    // obvious algorithm to determine whether the game has
    // been reduced to an endless cycle.
    return true;
}

CmdController::CmdController(Player p): Controller(p) {}

int CmdController::ChoosePit(Gameboard &board) {
    char user_input;
    int chosen_pit;
    bool valid_input;
    std::string warning_message = "";
    PitSowableState sowable_state;
    bool playing_p1 = (player == PlayerOne);

    do {
        DisplayPlayingGameboard(board, player, warning_message);
        std::cin >> user_input;

        // Validate and parse input
        if(std::cin.fail()) {
            // Closing stdin in game is interpreted as surrender
            if(std::cin.eof()) {
                std::cin.clear();
                return SURRENDER;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            warning_message = "The given input is invalid.";
            valid_input = false;
            continue;
        } 

        if(std::cin.peek() != '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            warning_message = "The given input is invalid.";
            valid_input = false;
            continue;
        } 
        
        if(user_input == QuitChar(player)) {
            std::cout << "Are you sure? (" << (playing_p1? "y/n" : "Y/N") << "): ";

            if(PromptYesNo(player)) {
                return SURRENDER;
            } else {
                valid_input = false;
                continue;
            }
        }

        if(user_input == ClaimEndlessCycleChar(player)) {
            // Wikipedia specifies that the game only ends due to both players agreeing
            // that it has been reduced to an endless cycle if each player has seeds in 
            // their pits.
            if(board.HasSeeds(PlayerOne) && board.HasSeeds(PlayerTwo)) {
                return CLAIM_ENDLESS_CYCLE;
            } else {
                warning_message = "You may only claim an endless cycle when both players have seeds in their pits.";
                valid_input = false;
                continue;
            }
        }

        if(user_input == QuitChar(Opponent(player))) {
            warning_message = "'";
            warning_message.push_back(user_input);
            warning_message.append("' is Player ");
            warning_message.push_back(playing_p1? '2' : '1');
            warning_message.append("'s surrender command.");
            valid_input = false;
            continue;
        }

        if(user_input == ClaimEndlessCycleChar(Opponent(player))) {
            warning_message = "'";
            warning_message.push_back(user_input);
            warning_message.append("' is Player ");
            warning_message.push_back(playing_p1? '2' : '1');
            warning_message.append("'s claim endless cycle command.");
            valid_input = false;
            continue;
        }

        chosen_pit = CharToPit(user_input);
        if(chosen_pit == -1) {
            warning_message = "The given input is invalid.";
            valid_input = false;
            continue;
        }

        valid_input = true;
        sowable_state = board.Sowable(player, chosen_pit);

        switch(sowable_state) {
            case PitInOpponentZone:
                warning_message = "Pit '"; 
                warning_message.push_back(user_input);
                warning_message.append("' does not belong to Player ");
                warning_message.push_back(playing_p1? '1' : '2');
                warning_message.append(".");
                break;
            case EmptyPit:
                warning_message = "Pit '";
                warning_message.push_back(user_input);
                warning_message.append("' is empty.");
                break;
            case OpponentOutOfSeeds:
                warning_message = "Player ";
                warning_message.push_back(playing_p1? '2' : '1'); 
                warning_message.append(" is out of seeds. Your next move must give him some.");
                break; 
        }
    } while(!valid_input || sowable_state != ValidPit);

    return chosen_pit;
}

bool CmdController::ask_endless_cycle() {
    DrawPlayerLabel(Opponent(player));
    setcolor(TEXT_COLOR);
    std::cout << " claims that the game has been reduced to an endless cycle." << std::endl;
    DrawPlayerLabel(player);
    setcolor(TEXT_COLOR);
    std::cout << ", do you agree? (" << (player == PlayerOne? "y/n" : "Y/N") << "): ";

    return PromptYesNo(player);
}
