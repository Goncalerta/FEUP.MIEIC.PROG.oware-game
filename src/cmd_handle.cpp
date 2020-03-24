#include "cmd_handle.h"
#include "cmd_utils.h"
#include <iostream>
#include <chrono>
#include <thread>

CmdHandle::CmdHandle() {}

void CmdHandle::Draw(Gameboard &board) {
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

int CmdHandle::ChoosePit(Player p, Gameboard &board) {
    gotoxy(0, 8);
    bool playing_p1 = p == PlayerOne;
    std::cout << "Player " << (playing_p1? 1 : 2) << " is playing this turn." << std::endl;

    char user_input;
    int chosen_pit;
    bool valid_input;
    PitSowableState sowable_state;

    gotoxy(0, 8+1);
    do {
        valid_input = false;
        std::cout << "Input pit letter (" << (playing_p1? "a-f" : "A-F") << "): ";
        std::cin >> user_input;

        if(std::cin.fail() || std::cin.peek() != '\n') {
            if(std::cin.eof()) {
                // TODO stream closed
            }
            
            // TODO better number than 100000 std::numeric_limits<std::streamsize>
            std::cin.ignore(100000, '\n');
            gotoxy(0, 8+1);
            std::cout << "The given input is invalid." << std::endl;
            continue;
        }

        chosen_pit = CharToPit(user_input);
        if(chosen_pit == -1) {
            gotoxy(0, 8+1);
            std::cout << "The given input is invalid." << std::endl;
            continue;
        }

        valid_input = true;
        sowable_state = board.Sowable(p, chosen_pit);

        switch(sowable_state) {
            case PitInOpponentZone:
                gotoxy(0, 8+1);
                std::cout << "Pit `" << user_input << "` does not belong to player " << (playing_p1? 1 : 2) << "." << std::endl;
                break;
            case EmptyPit:
                gotoxy(0, 8+1);
                std::cout << "Pit `" << user_input << "` is empty." << std::endl;
                break;
            case OpponentOutOfSeeds:
                gotoxy(0, 8+1);
                std::cout << "Player " << (playing_p1? 2 : 1) << " is out of seeds. Your next move must give him some." << std::endl;
                break; 
        }
    } while(!valid_input || sowable_state != ValidPit);

    return chosen_pit;
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
