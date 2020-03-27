#include "controller.h"
#include "cmd_utils.h"
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

// TODO this should be possible to join with actual move
Gameboard SimulateMove(Gameboard &board, int pit) {
    Gameboard simulation = board;
    Player current_player = PitZone(pit);
    
    int last_sowed = simulation.Sow(pit, NULL);

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

    simulation.Capture(PlayerOne, PlayerBoard(PlayerOne));
    simulation.Capture(PlayerTwo, PlayerBoard(PlayerTwo));

    return simulation;
}

int BiggestCaptureMoveScore(Gameboard &board, Player p) {
    Range player_pits = PlayerBoard(p);
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
    Range player_pits = PlayerBoard(player);
    Range opponent_pits = PlayerBoard(Opponent(player));

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

        if(lean > choice_lean || (lean == choice_lean && rand()%2 == 0)) {
            choice_lean = lean;
            choice = pit;
        }
    }

    return choice;
}

bool BotController::ask_endless_cycle() {
    return true;
}

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

char QuitChar(Player p) {
    return p == PlayerOne? 'q' : 'Q';
}

char ClaimEndlessCycleChar(Player p) {
    return p == PlayerOne? 'p' : 'P';
}

CmdController::CmdController(Player p): Controller(p) {}

int CmdController::ChoosePit(Gameboard &board) {
    char user_input;
    int chosen_pit;
    bool valid_input;
    std::string warning_message = "";
    PitSowableState sowable_state;

    do {
        // Draw board and information about the state of the game
        // before asking user for input
        bool playing_p1 = player == PlayerOne;

        gotoxy(0, 8);
        std::cout << "Player " << (playing_p1? 1 : 2) << " is playing this turn." << std::endl;
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
        
        if(user_input == QuitChar(player)) return SURRENDER;
        if(user_input == ClaimEndlessCycleChar(player)) return CLAIM_ENDLESS_CYCLE;
        if(user_input == QuitChar(Opponent(player))) {
            warning_message = "'";
            warning_message.push_back(user_input);
            warning_message.append("' is player ");
            warning_message.push_back(playing_p1? '1' : '2');
            warning_message.append("'s surrender command.");
            valid_input = false;
            continue;
        }
        if(user_input == ClaimEndlessCycleChar(Opponent(player))) {
            warning_message = "'";
            warning_message.push_back(user_input);
            warning_message.append("' is player ");
            warning_message.push_back(playing_p1? '1' : '2');
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
                warning_message.append("' does not belong to player ");
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
    char answer;
    char agree_char = player == PlayerOne? 'p' : 'P';
    char opponent_agree_char = player == PlayerOne? 'P' : 'p';

    std::cout << "Player " << (player == PlayerOne? "2" : "1") << " claims that the game has been reduced to an endless cycle." << std::endl;
    std::cout << "Player " << (player == PlayerOne? "1" : "2") << ", input '" << agree_char << "' to agree, or any other letter otherwise: ";

    // The prompt will only be repeated in case the player uses opponent's agree character
    // that is because they are very similiar, and so could be a source of confusion.
    while(true) {
        std::cin >> answer;

        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return false;
        } 

        if(answer == opponent_agree_char) {
            std::cout << "'" << opponent_agree_char << "' is Player " << (player == PlayerOne? "2" : "1")
                << "'s agree command. If you intend to agree, please input '" << agree_char 
                << "'. Otherwise input any other letter: ";
        } else {
            return answer == agree_char;
        }
    } 
}
