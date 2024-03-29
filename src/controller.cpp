#include "controller.h"
#include "cmd_handle.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>  
#include <ctime> 

BotController::BotController(Player p): Controller(p) {}

int BotController::BestMoveScore(Gameboard &board, Player p) {
    Range player_pits = PlayerZone(p);
    int score = 0;
    
    if(board.HasLegalMove(p)){
        for(int pit = player_pits.begin; pit <= player_pits.end; pit++) {
            if(board.Sowable(p, pit) != ValidPit) continue;
            
            Gameboard simulation = board;
            simulation.PlayMove(pit, p); 

            int new_score = simulation.PlayerScore(p);
            if(new_score >= 25) return new_score;

            if(new_score > score) {
                new_score = score;
            }
        }
    } else {
        // out of moves means that the player would capture their seeds
        Gameboard simulation = board;
        simulation.Capture(p, PlayerZone(p));

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
    // payoff is negative, as the calculated payoff will always be
    // positive.
    int choice = SURRENDER;
    int choice_payoff = -1;
    
    // Simulate legal moves to find out which one gives the biggest
    // immediate payoff.
    for(int pit = player_zone.begin; pit <= player_zone.end; pit++) {
        if(board.Sowable(player, pit) != ValidPit) continue;
        // Any legal move makes more sense that surrendering, even
        // if it is a guaranteed lost.
        if(choice == SURRENDER) choice = pit;

        // copies the `Gameboard` to experiment simulations of moves
        Gameboard simulation = board; 
        simulation.PlayMove(pit, player); 

        int new_score = simulation.PlayerScore(player);
        if(new_score >= 25) {
            choice = pit; // Always choose winning moves
            break;
        }

        int opponent_new_score = BestMoveScore(simulation, Opponent(player));
        if(opponent_new_score >= 25) continue; // Always avoid losing moves

        // `payoff` is a positive integer that reflects how good this play is 
        // in terms of immediate payoff.
        int delta_player = new_score - old_score;
        int delta_opponent = opponent_new_score - opponent_old_score;
        int payoff = new_score * delta_player - opponent_new_score * delta_opponent;
        // This random factor will distinguish moves with the same (or very similar) `payoff`.
        int rand_factor = rand()%200;
        // Scale by 100 so `rand_factor` doesn't dominate the choice; add 62500 so the value
        // is guaranteed to be positive.
        payoff = payoff*100 + 62500 + rand_factor;
        
        if(payoff > choice_payoff) {
            choice_payoff = payoff;
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
            // EOF in game is interpreted as surrender
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
