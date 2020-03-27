#include <iostream>
#include "game.h"
#include "cmd_utils.h"
#include <limits>
#include <iomanip>
#include <chrono>
#include <thread>

// TODO bot
// TODO documentation and code cleanup

const char title_screen[] = "   ___                         \n"
                            "  / _ \\__      ____ _ _ __ ___ \n"
                            " | | | \\ \\ /\\ / / _` | '__/ _ \\\n"
                            " | |_| |\\ V  V / (_| | | |  __/\n"
                            "  \\___/  \\_/\\_/ \\__,_|_|  \\___|\n";

void draw_menu() {
    std::cout << title_screen << std::endl << std::endl;
    std::cout << "    [1] Player Vs. Player" << std::endl;
    std::cout << "    [2] Player Vs. AI" << std::endl;
    std::cout << "    [3] Exit game" << std::endl << std::endl;
}

enum MenuOption {
    PlayerVsPlayer = 1,
    PlayerVsAi = 2,
    ExitGame = 3,
};

MenuOption prompt_player() {
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

// TODO print extra info based on whether someone surrendered
void draw_gameover_screen(Game &game) {
    std::cout << title_screen << std::endl << std::endl;
    switch(game.win_state) {
        case PlayerOneWins:
            std::cout << "      Player 1 wins!" << std::endl;
            break;
        case PlayerTwoWins:
            std::cout << "      Player 2 wins!" << std::endl;
            break;
        case Draw:
            std::cout << "      Draw." << std::endl;
            break;
    }
    std::cout << "Player 1 score: " << game.board.p1_score << "\t";
    std::cout << "Player 2 score: " << game.board.p2_score << std::endl;
    // TODO ignore input while sleeping
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

int main() {
    Game game;
    Controller *p1_controller, *p2_controller;

    while(true) {
        clrscr();
        draw_menu();
        switch(prompt_player()) {
            case PlayerVsPlayer:
                game = Game();
                p1_controller = new CmdController(PlayerOne);
                p2_controller = new CmdController(PlayerTwo);

                while(game.win_state == OngoingGame) {
                    game.PlayTurn(p1_controller, p2_controller);
                }
                clrscr();
                draw_gameover_screen(game);
                break;
            case PlayerVsAi:
                game = Game();
                p1_controller = new CmdController(PlayerOne);
                p2_controller = new BotController(PlayerTwo);

                while(game.win_state == OngoingGame) {
                    game.PlayTurn(p1_controller, p2_controller);
                }
                clrscr();
                draw_gameover_screen(game);
                break;
            case ExitGame:
                return 0;
        }
    }
}
