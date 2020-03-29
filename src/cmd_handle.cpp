#include "cmd_handle.h"
#include "cmd_utils.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <ios>
#include <limits>
#include <string>
#include <iomanip>

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

char PitToChar(int pit) {
    switch(pit) {
        case 0: return 'a';
        case 1: return 'b';
        case 2: return 'c';
        case 3: return 'd';
        case 4: return 'e';
        case 5: return 'f';
        case 6: return 'A';
        case 7: return 'B';
        case 8: return 'C';
        case 9: return 'D';
        case 10: return 'E';
        case 11: return 'F';
        default: return ' ';
    }
}

void DrawTitleScreen() {
    setcolor(TITLE_SCREEN_COLOR);
    std::cout << TITLE_SCREEN << std::endl;
}

void DrawMenuOption(int number, std::string option) {
    setcolor(MENU_NUMBER_COLOR);
    std::cout << "    [" << number << "] ";
    setcolor(TEXT_COLOR);
    std::cout << option << std::endl;
}

void DrawMenu() {
    DrawTitleScreen();
    DrawMenuOption(1, "Player Vs. Player");
    DrawMenuOption(2, "Player Vs. AI");
    DrawMenuOption(3, "Exit game");
    std::cout << std::endl;
}

MenuOption MenuPrompt() {
    char user_input;

    setcolor(TEXT_COLOR);

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

void DrawPlayerLabel(Player p) {
    if(p == PlayerOne) {
        setcolor(PLAYER_ONE_COLOR);
        std::cout << "Player 1";
    } else {
        setcolor(PLAYER_TWO_COLOR);
        std::cout << "Player 2";
    }
}

void DrawGameoverScreen(Game &game) {
    DrawTitleScreen();

    switch(game.win_state) {
        case PlayerOneWins:
            std::cout << "    ";
            DrawPlayerLabel(PlayerOne);
            setcolor(TEXT_COLOR);
            std::cout << " wins!" << std::endl;

            if(game.surrendered) {
                std::cout << "    ";
                DrawPlayerLabel(PlayerTwo);
                setcolor(TEXT_COLOR);
                std::cout << " surrendered." << std::endl;
            }

            std::cout << std::endl;
            break;
        case PlayerTwoWins:
            std::cout << "    ";
            DrawPlayerLabel(PlayerTwo);
            setcolor(TEXT_COLOR);
            std::cout << " wins!" << std::endl;

            if(game.surrendered) {
                std::cout << "    ";
                DrawPlayerLabel(PlayerOne);
                setcolor(TEXT_COLOR);
                std::cout << " surrendered." << std::endl;
            }

            std::cout << std::endl;
            break;
        case Draw:
            setcolor(PLAYER_NEUTRAL_COLOR);
            std::cout << "      Draw." << std::endl << std::endl;
            break;
    }
    
    setcolor(TEXT_COLOR);
    std::cout << "Player 1 score: ";
    setcolor(PLAYER_ONE_COLOR);
    std::cout << game.board.p1_score << "\t";
    

    setcolor(TEXT_COLOR);
    std::cout << "Player 2 score: ";
    setcolor(PLAYER_TWO_COLOR);
    std::cout << game.board.p2_score << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(GAMEOVER_SCREEN_MILLISECONDS));
}

void DrawGameboard(Gameboard &board) {
    // TODO prettify  (show how to surrender and claim endless cycle)
    // TODO [prettify] update score animation

    setcolor(PLAYER_TWO_COLOR);
    std::cout << "               ";
    for(int i = 11; i >= 6; i--) {
        if(board.Sowable(PlayerTwo, i) == ValidPit) {
            setcolor(PLAYER_TWO_SOWABLE_COLOR);
        } else {
            setcolor(PLAYER_TWO_UNSOWABLE_COLOR);
        } 

        std::cout << PitToChar(i) << "   ";
    }
    std::cout << std::endl;
    DrawPlayerLabel(PlayerTwo);
    std::cout << "      ";
    for(int i = 11; i >= 6; i--) {
        if(board.Sowable(PlayerTwo, i) == ValidPit) {
            setcolor(PLAYER_TWO_SOWABLE_COLOR);
        } else {
            setcolor(PLAYER_TWO_UNSOWABLE_COLOR);
        } 

        std::cout << std::setw(2) << board.pits[i] << "  ";
    }
    setcolor(TEXT_COLOR);
    std::cout << "   Score: ";
    setcolor(PLAYER_TWO_COLOR);
    std::cout << std::setw(2) << board.p2_score;
    setcolor(PLAYER_NEUTRAL_COLOR);
    std::cout << std::endl << "             ---------------------------------------" << std::endl;
    DrawPlayerLabel(PlayerOne);
    std::cout << "      ";
    for(int i = 0; i <= 5; i++) {
        if(board.Sowable(PlayerOne, i) == ValidPit) {
            setcolor(PLAYER_ONE_SOWABLE_COLOR);
        } else {
            setcolor(PLAYER_ONE_UNSOWABLE_COLOR);
        } 

        std::cout << std::setw(2) << board.pits[i] << "  ";
    }
    setcolor(TEXT_COLOR);
    std::cout << "   Score: ";
    setcolor(PLAYER_ONE_COLOR);
    std::cout << std::setw(2) << board.p1_score << std::endl;
    std::cout << "               ";
    for(int i = 0; i <= 5; i++) {
        if(board.Sowable(PlayerOne, i) == ValidPit) {
            setcolor(PLAYER_ONE_SOWABLE_COLOR);
        } else {
            setcolor(PLAYER_ONE_UNSOWABLE_COLOR);
        } 

        std::cout << PitToChar(i) << "   ";
    }
    std::cout << std::endl;
}

// TODO [cleanup] global variables, name
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

// TODO [cleanup] global variable
void SetPit(int pit, int value) {
    set_cursor_on_pit(pit);
    setcolor(PLAYER_NEUTRAL_COLOR);
    std::cout << std::setw(2) << value;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void HighlightCapture(Range capture, Player p, Gameboard &board) {
    int player_color = p == PlayerOne? PLAYER_ONE_COLOR : PLAYER_TWO_COLOR;
    setcolor(WHITE, player_color);
    for(int i = capture.begin; i <= capture.end; i++) {
        set_cursor_on_pit(i);
        std::cout << std::setw(2) << board.pits[i];
    }
    setcolor(TEXT_COLOR, BLACK);
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
}
