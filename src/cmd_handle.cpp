#include "cmd_handle.h"
#include "cmd_utils.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <ios>
#include <limits>
#include <string>
#include <iomanip>

void DrawMenu() {
    clrscr();
    std::cout << TITLE_SCREEN << std::endl << std::endl;
    std::cout << "    [1] Player Vs. Player" << std::endl;
    std::cout << "    [2] Player Vs. AI" << std::endl;
    std::cout << "    [3] Exit game" << std::endl << std::endl;
}

MenuOption MenuPrompt() {
    char user_input;

    while(true) {
        std::cout << "Please input an option number: ";
        std::cin >> user_input;
        if(std::cin.fail()) {
            // Closing stdin in menu is interpreted as exit game
            if(std::cin.eof()) return ExitGame;

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input." << std::endl;
            continue;
        } 

        if(std::cin.peek() != '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input." << std::endl;
            continue;
        } 
        
        switch(user_input) {
            case '1': return PlayerVsPlayer;
            case '2': return PlayerVsAi;
            case '3': return ExitGame;
            default: std::cout << "Invalid input." << std::endl;
        } 
    }
}

void DrawGameoverScreen(Game &game) {
    clrscr();
    std::cout << TITLE_SCREEN << std::endl;
    switch(game.win_state) {
        case PlayerOneWins:
            std::cout << "      Player 1 wins!" << std::endl;
            if(game.surrendered) std::cout << "      Player 2 surrendered." << std::endl;
            std::cout << std::endl;
            break;
        case PlayerTwoWins:
            std::cout << "      Player 2 wins!" << std::endl; 
            if(game.surrendered) std::cout << "      Player 1 surrendered." << std::endl;
            std::cout << std::endl;
            break;
        case Draw:
            std::cout << "      Draw." << std::endl << std::endl;
            break;
    }
    std::cout << "Player 1 score: " << game.board.p1_score << "\t";
    std::cout << "Player 2 score: " << game.board.p2_score << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

void DrawGameboard(Gameboard &board) {
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
