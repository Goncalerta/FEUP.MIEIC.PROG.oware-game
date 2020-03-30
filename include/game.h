#ifndef GAME_H
#define GAME_H

struct Game;

#include "gameboard.h"
#include "player.h"
#include "controller.h"

// Represents either that the game isn't over yet or the winner.
enum WinState {
    // The game is still ongoing. No winner determined.
    OngoingGame,
    // Player 1 wins.
    PlayerOneWins,
    // Player 2 wins.
    PlayerTwoWins,
    // The game ended in a draw.
    Draw,
};

// Holds all game information.
struct Game {
    // The `Gameboard`.
    Gameboard board;
    // The `Player` that is playing the next turn.
    Player current_player;
    // Either the winner or the game isn't over yet.
    WinState win_state;
    // Whether one of the players surrendered.
    bool surrendered;

    // Starts a new game.
    //
    // The first `Player` to make a move is Player 1.
    Game();

    // Plays the next turn of the `Game`.
    //
    // `p1_controller` handles the interactions between the `Game` and Player 1.
    // `p2_controller` handles the interactions between the `Game` and Player 2.
    void PlayTurn(Controller *p1_controller, Controller *p2_controller);

    // Plays the move of sowing the given `pit`. Must be a playable move, so
    // the pit must be valid (between 0 and 11) and sowable 
    // (`Gameboard::Sowable(current_player, pit)` must return `ValidPit`).
    void PlayMove(int pit);

    // Ends the game as the `Game::current_player` can't make any legal move.
    //
    // The `Player` will capture all seeds in owned pits. 
    void OutOfMoves();

    // Ends the game as both players agreed it has been reduced to an endless cycle.
    //
    // Both players capture all seeds in their owned pits.
    void EndlessCycle();

    // Ends the game as `Player p` surrendered.
    //
    // The opponent automatically wins.
    void Surrender(Player p);

    // Updates `Game::win_state` based on the current score of the players.
    //
    // A player that has captured 25 or more seeds wins the `Game`. If both 
    // players have captured 24, the game ends in a draw.
    void UpdateWinState();
};

#endif
