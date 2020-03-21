#include "cmd_handle.h"
#include "cmd_utils.h"
#include <iostream>

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

int CmdHandle::ChoosePit(Game &game) {
    char user_input;

    std::cout << std::endl << std::endl;
    bool playing_p1 = game.current_player == PlayerOne;
    std::cout << "Player " << (playing_p1? 1 : 2) << " is playing this turn." << std::endl;
    std::cout << "Input pit letter (" << (playing_p1? "a-f" : "A-F") << "): ";
    // TODO improve error handling for invalid inputs (ex. multiple inputs separated by space, non chars etc)
    std::cin >> user_input;
    return CharToPit(user_input);
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
