#include "controller.h"
#include "cmd_handle.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <ios>
#include <limits>
#include <string>
#include <iomanip>
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

Gameboard SimulateOutOfMoves(Gameboard &board) {
    Gameboard simulation = board;

    simulation.Capture(PlayerOne, PlayerZone(PlayerOne));
    simulation.Capture(PlayerTwo, PlayerZone(PlayerTwo));

    return simulation;
}

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
    clrscr();
    DrawGameboard(board, true, false);

    gotoxy(0, GAME_PROMPT_LINE);
    DrawPlayerLabel(player);
    setcolor(TEXT_COLOR);
    std::cout << " is playing this turn." << std::endl;
    DrawPlayerLabel(player);
    setcolor(TEXT_COLOR);
    std::cout << " is thinking... ";

    Range player_pits = PlayerZone(player);
    Range opponent_pits = PlayerZone(Opponent(player));

    int old_score = board.PlayerScore(player);
    int opponent_old_score = board.PlayerScore(Opponent(player));

    int choice = SURRENDER;
    int choice_lean = -1;
    
    for(int pit = player_pits.begin; pit <= player_pits.end; pit++) {
        if(board.Sowable(player, pit) != ValidPit) continue;
        // Any legal move makes more sense that surrendering
        if(choice == SURRENDER) choice = pit;

        Gameboard simulation = SimulateMove(board, pit);

        int new_score = simulation.PlayerScore(player);
        if(new_score >= 25) return pit;

        int opponent_new_score = BiggestCaptureMoveScore(simulation, Opponent(player));
        if(opponent_new_score >= 25) continue;

        int lean = ((new_score - old_score) - (opponent_new_score - opponent_old_score)) + 25;

        // TODO this doesn't give equal probability to every choice
        if(lean > choice_lean || (lean == choice_lean && rand()%2 == 0)) {
            choice_lean = lean;
            choice = pit;
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    std::cout << PitToChar(choice) << std::endl;
    return choice;
}

bool BotController::ask_endless_cycle() {
    return true;
}

CmdController::CmdController(Player p): Controller(p) {}

bool PromptYesNo(Player p) {
    char answer;
    char agree_char = p == PlayerOne? 'y' : 'Y';
    char disagree_char = p == PlayerOne? 'n' : 'N';
    char opponent_agree_char = p == PlayerOne? 'Y' : 'y';
    char opponent_disagree_char = p == PlayerOne? 'N' : 'n';
    const char *valid_inputs = p == PlayerOne? "y/n" : "Y/N";

    while(true) {
        std::cin >> answer;

        if(std::cin.fail()) {
            // Closing stdin in prompt is interpreted as a 'no'
            if(std::cin.eof()) return false;

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Input (" << valid_inputs << "): ";

        } else if(answer == opponent_agree_char) {
            std::cout << "'" << opponent_agree_char << "' is Player " << (p == PlayerOne? "2" : "1")
                << "'s agree command. Input (" << valid_inputs << "): ";
        
        } else if(answer == opponent_disagree_char) {
            std::cout << "'" << opponent_disagree_char << "' is Player " << (p == PlayerOne? "2" : "1")
                << "'s disagree command. Input (" << valid_inputs << "): ";
        
        } else if(answer == agree_char) {
            return true;
        
        } else if(answer == disagree_char) {
            return false;
        
        } else {
            std::cout << "Invalid input. Input (" << valid_inputs << "): ";
        }
    }
}

int CmdController::ChoosePit(Gameboard &board) {
    char user_input;
    int chosen_pit;
    bool valid_input;
    std::string warning_message = "";
    PitSowableState sowable_state;
    bool playing_p1 = player == PlayerOne;

    do {
        clrscr();
        DrawGameboard(board, playing_p1, !playing_p1);
        
        gotoxy(0, GAME_PROMPT_LINE);
        DrawPlayerLabel(player);
        setcolor(TEXT_COLOR);
        std::cout << " is playing this turn." << std::endl;       
        if(warning_message != "") std::cout << warning_message << std::endl;
        std::cout << "Input pit letter (" << (playing_p1? "a-f" : "A-F") << "): ";
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
            std::cout << "Are you sure? (" << (player == PlayerOne? "y/n" : "Y/N") << "): ";

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
