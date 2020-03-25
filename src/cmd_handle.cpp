#define NOMINMAX
#include "cmd_handle.h"
#include "cmd_utils.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <ios>
#include <limits>
#include <string>

CmdHandle::CmdHandle() {}

void CmdHandle::Draw(Gameboard &board) {
    // TODO prettify (color for each opponent, for avaliable and not avaliable pits, etc) (double digits)
    clrscr();
    std::cout << "              F  E  D  C  B  A" << std::endl;
    std::cout << "Player Two    ";
    for(int i = 11; i >= 6; i--) {
        std::cout << board.pits[i] << "  ";
    }
    std::cout << "   Score: " << board.p2_score;
    std::cout << std::endl << "         -----------------------------" << std::endl;
    std::cout << "Player One    ";
    for(int i = 0; i <= 5; i++) {
        std::cout << board.pits[i] << "  ";
    }
    std::cout << "   Score: " << board.p1_score;
    std::cout << std::endl << "              a  b  c  d  e  f" << std::endl;
}

void CmdHandle::SetPit(int pit, int value) {
    int x, y;
    if(pit < 6) {
        y = 3;
        x = 14 + 3*pit;
    } else {
        y = 1;
        x = 14 + 3*(11 - pit);
    }

    gotoxy(x, y);
    std::cout << value;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
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

int CmdHandle::ChoosePit(Player p, Gameboard &board) {
    char user_input;
    int chosen_pit;
    bool valid_input;
    std::string warning_message = "";
    PitSowableState sowable_state;

    do {
        // Draw board and information about the state of the game
        // before asking user for input
        bool playing_p1 = p == PlayerOne;

        Draw(board);
        gotoxy(0, 8);
        std::cout << "Player " << (playing_p1? 1 : 2) << " is playing this turn." << std::endl;
        if(warning_message != "") std::cout << warning_message << std::endl;
        std::cout << "Input pit letter (" << (playing_p1? "a-f" : "A-F") << "): ";
        std::cin >> user_input;

        // Validate and parse input
        if(std::cin.fail()) {
            if(std::cin.eof()) {
                // TODO what should happen if stream closed? (Ctrl-z and then ENTER)
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
        
        chosen_pit = CharToPit(user_input);
        if(chosen_pit == -1) {
            warning_message = "The given input is invalid.";
            valid_input = false;
            continue;
        }

        valid_input = true;
        sowable_state = board.Sowable(p, chosen_pit);

        switch(sowable_state) {
            case PitInOpponentZone:
                warning_message = "Pit `"; 
                warning_message.push_back(user_input);
                warning_message.append("` does not belong to player ");
                warning_message.push_back(playing_p1? '1' : '2');
                warning_message.append(".");
                break;
            case EmptyPit:
                warning_message = "Pit `";
                warning_message.push_back(user_input);
                warning_message.append("` is empty.");
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

void set_cursor_on_pit(int pit) {
    int x, y;
    if(pit < 6) {
        y = 3;
        x = 14 + 3*pit;
    } else {
        y = 1;
        x = 14 + 3*(11 - pit);
    }
    gotoxy(x, y);
}

void CmdHandle::HighlightCapture(Range capture, Gameboard &board) {
    setcolor(BLACK, RED);
    for(int i = capture.begin; i <= capture.end; i++) {
        set_cursor_on_pit(i);
        std::cout << board.pits[i];
    }
    setcolor(LIGHTGRAY, BLACK);
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
}
