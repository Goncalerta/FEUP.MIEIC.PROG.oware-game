#include "cmd_handle.h"
#include "cmd_utils.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <ios>
#include <limits>
#include <string>
#include <iomanip>

void DrawGame(Gameboard &board) {
    // TODO prettify (color for each opponent, for avaliable and not avaliable pits, etc) (show how to surrender and claim endless cycle)
    // TODO [prettify] update score animation
    clrscr();
    std::cout << "               F   E   D   C   B   A" << std::endl;
    std::cout << "Player Two    ";
    for(int i = 11; i >= 6; i--) {
        std::cout << std::setw(2) << board.pits[i] << "  ";
    }
    std::cout << "   Score: " << std::setw(2) << board.p2_score;
    std::cout << std::endl << "     ----------------------------------------" << std::endl;
    std::cout << "Player One    ";
    for(int i = 0; i <= 5; i++) {
        std::cout << std::setw(2) << board.pits[i] << "  ";
    }
    std::cout << "   Score: " << std::setw(2) << board.p1_score;
    std::cout << std::endl << "               a   b   c   d   e   f" << std::endl;
}

void SetPit(int pit, int value) {
    int x, y;
    if(pit < 6) {
        y = 3;
        x = 14 + 4*pit;
    } else {
        y = 1;
        x = 14 + 4*(11 - pit);
    }

    gotoxy(x, y);
    std::cout << std::setw(2) << value;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void set_cursor_on_pit(int pit) {
    int x, y;
    if(pit < 6) {
        y = 3;
        x = 14 + 4*pit;
    } else {
        y = 1;
        x = 14 + 4*(11 - pit);
    }
    gotoxy(x, y);
}

void HighlightCapture(Range capture, Gameboard &board) {
    setcolor(BLACK, RED);
    for(int i = capture.begin; i <= capture.end; i++) {
        set_cursor_on_pit(i);
        std::cout << std::setw(2) << board.pits[i];
    }
    setcolor(LIGHTGRAY, BLACK);
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
}

