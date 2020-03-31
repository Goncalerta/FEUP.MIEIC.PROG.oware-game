#include "cmd_handle.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <ios>
#include <limits>
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

char QuitChar(Player p) {
    return p == PlayerOne? 'q' : 'Q';
}

char ClaimEndlessCycleChar(Player p) {
    return p == PlayerOne? 'p' : 'P';
}

char AgreeChar(Player p) {
    return p == PlayerOne? 'y' : 'Y';
}

char DisagreeChar(Player p) {
    return p == PlayerOne? 'n' : 'N';
}

void DrawPlayerLabel(Player p, bool on) {
    if(p == PlayerOne) {
        if(on) setcolor(PLAYER_ONE_COLOR);
        else setcolor(PLAYER_ONE_OFF_COLOR);

        std::cout << "Player 1";
    } else {
        if(on) setcolor(PLAYER_TWO_COLOR);
        else setcolor(PLAYER_TWO_OFF_COLOR);

        std::cout << "Player 2";
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

void DisplayMenu() {
    clrscr();
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
            // EOF in menu is interpreted as exit game
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

void DisplayGameoverScreen(Game &game) {
    clrscr();
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

void DrawGameboard(Gameboard &board, Player current_player, bool p1_controls, bool p2_controls) {
    // 'A B C D E F' line
    setcolor(PLAYER_TWO_COLOR);
    std::cout << "               ";
    for(int i = 11; i >= 6; i--) {
        if(board.Sowable(PlayerTwo, i) == ValidPit) {
            setcolor(PLAYER_TWO_COLOR);
        } else {
            setcolor(PLAYER_TWO_OFF_COLOR);
        } 

        std::cout << PitToChar(i) << "   ";
    }

    if(p2_controls) {
        setcolor(TEXT_COLOR);
        std::cout << "                 " << "Surrender: ";
        setcolor(PLAYER_TWO_COLOR);
        std::cout << "'" << QuitChar(PlayerTwo) << "'";
    }

    std::cout << std::endl;
    
    // p2 pits line
    DrawPlayerLabel(PlayerTwo, current_player == PlayerTwo);
    std::cout << "      ";
    for(int i = 11; i >= 6; i--) {
        if(board.Sowable(PlayerTwo, i) == ValidPit) {
            setcolor(PLAYER_TWO_COLOR);
        } else {
            setcolor(PLAYER_TWO_OFF_COLOR);
        } 

        std::cout << std::setw(2) << board.pits[i] << "  ";
    }
    setcolor(TEXT_COLOR);
    std::cout << "   Score: ";
    setcolor(PLAYER_TWO_COLOR);
    std::cout << std::setw(2) << board.p2_score;

    if(p2_controls) {
        setcolor(TEXT_COLOR);
        std::cout << "      " << "Claim endless cycle: ";
        setcolor(PLAYER_TWO_COLOR);
        std::cout << "'" << ClaimEndlessCycleChar(PlayerTwo) <<"'";
    }
    
    // separation line
    setcolor(PLAYER_NEUTRAL_COLOR);
    std::cout << std::endl << "             ---------------------------------------" << std::endl;
    
    // p1 pits line
    DrawPlayerLabel(PlayerOne, current_player == PlayerOne);
    std::cout << "      ";
    for(int i = 0; i <= 5; i++) {
        if(board.Sowable(PlayerOne, i) == ValidPit) {
            setcolor(PLAYER_ONE_COLOR);
        } else {
            setcolor(PLAYER_ONE_OFF_COLOR);
        } 

        std::cout << std::setw(2) << board.pits[i] << "  ";
    }
    setcolor(TEXT_COLOR);
    std::cout << "   Score: ";
    setcolor(PLAYER_ONE_COLOR);
    std::cout << std::setw(2) << board.p1_score;
    
    if(p1_controls) {
        setcolor(TEXT_COLOR);
        std::cout << "      " << "Surrender: ";
        setcolor(PLAYER_ONE_COLOR);
        std::cout << "'" << QuitChar(PlayerOne) << "'";
    }
    
    // 'a b c d e f' line
    std::cout << std::endl << "               ";
    for(int i = 0; i <= 5; i++) {
        if(board.Sowable(PlayerOne, i) == ValidPit) {
            setcolor(PLAYER_ONE_COLOR);
        } else {
            setcolor(PLAYER_ONE_OFF_COLOR);
        } 

        std::cout << PitToChar(i) << "   ";
    }
    
    if(p1_controls) {
        setcolor(TEXT_COLOR);
        std::cout << "                 " << "Claim endless cycle: ";
        setcolor(PLAYER_ONE_COLOR);
        std::cout << "'" << ClaimEndlessCycleChar(PlayerOne) <<"'";
    }
    
    std::cout << std::endl;
}

void DisplayOutOfMovesGameboard(Gameboard &board, Player p) {
    clrscr();
    DrawGameboard(board, p, false, false);
    gotoxy(0, GAME_PROMPT_LINE);
    DrawPlayerLabel(p);
    setcolor(TEXT_COLOR);
    std::cout << " is out of legal moves.";
}

void DisplayPlayingGameboard(Gameboard &board, Player p, std::string warning_message) {
    bool playing_p1 = (p == PlayerOne);

    clrscr();
    DrawGameboard(board, p, playing_p1, !playing_p1);
    
    gotoxy(0, GAME_PROMPT_LINE);
    DrawPlayerLabel(p);
    setcolor(TEXT_COLOR);
    std::cout << " is playing this turn." << std::endl;       
    if(warning_message != "") std::cout << warning_message << std::endl;
    std::cout << "Input pit letter (" << (playing_p1? "a-f" : "A-F") << "): ";
}

void DisplayBotPlayingGameboard(Gameboard &board, Player p) {
    clrscr();
    DrawGameboard(board, p, true, false);

    gotoxy(0, GAME_PROMPT_LINE);
    DrawPlayerLabel(p);
    setcolor(TEXT_COLOR);
    std::cout << " is playing this turn." << std::endl;
    DrawPlayerLabel(p);
    setcolor(TEXT_COLOR);
    std::cout << " is thinking... ";
}

void SetCursorOnPit(int pit) {
    int x, y;
    if(pit < 6) {
        y = PLAYER_ONE_PITS_LINE;
        x = PITS_START_COLUMN + 4*pit;
    } else {
        y = PLAYER_TWO_PITS_LINE;
        x = PITS_START_COLUMN + 4*(11 - pit);
    }
    gotoxy(x, y);
}

void SetPit(int pit, int value) {
    SetCursorOnPit(pit);
    setcolor(PLAYER_NEUTRAL_COLOR);
    std::cout << std::setw(2) << value;
    std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_PIT_DELAY));
}

void SetScore(Player p, int value) {
    int x = PLAYER_SCORE_COLUMN;
    int y = (p == PlayerOne? PLAYER_ONE_PITS_LINE : PLAYER_TWO_PITS_LINE);
    gotoxy(x, y);
    setcolor(SET_SCORE_COLOR);
    std::cout << std::setw(2) << value;
}

void SetScoreAnimation(Player p, int old_value, int new_value) {
    for(int i = old_value; i <= new_value; i++) {
        SetScore(p, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_SCORE_TICK));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(SCORE_UPDATED_DELAY));
}

void HighlightCapture(Player p, Range capture, int *pits) {
    int player_color = (p == PlayerOne? PLAYER_ONE_COLOR : PLAYER_TWO_COLOR);
    setcolor(WHITE, player_color);
    
    for(int i = capture.begin; i <= capture.end; i++) {
        SetCursorOnPit(i);
        std::cout << std::setw(2) << pits[i];
    }
    
    setcolor(TEXT_COLOR, BLACK);
    std::this_thread::sleep_for(std::chrono::milliseconds(CAPTURE_DELAY));
}

bool PromptYesNo(Player p) {
    char answer;
    const char *valid_inputs = (p == PlayerOne? "y/n" : "Y/N");

    while(true) {
        std::cin >> answer;

        if(std::cin.fail()) {
            // EOF in prompt is interpreted as a 'no'
            if(std::cin.eof()) {
                std::cin.clear();
                return false;
            }

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Input (" << valid_inputs << "): ";

        } else if(answer == AgreeChar(Opponent(p))) {
            std::cout << "'" << AgreeChar(Opponent(p)) << "' is Player " 
                << (p == PlayerOne? "2" : "1")
                << "'s agree command. Input (" << valid_inputs << "): ";
        
        } else if(answer == DisagreeChar(Opponent(p))) {
            std::cout << "'" << DisagreeChar(Opponent(p)) << "' is Player " 
                << (p == PlayerOne? "2" : "1")
                << "'s disagree command. Input (" << valid_inputs << "): ";
        
        } else if(answer == AgreeChar(p)) {
            return true;
        
        } else if(answer == DisagreeChar(p)) {
            return false;
        
        } else {
            std::cout << "Invalid input. Input (" << valid_inputs << "): ";
        }
    }
}
