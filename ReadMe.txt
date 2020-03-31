==================================
=== OWARE GAME  -  PROG 2020   ===
=== By Pedro Gonçalo Correia   ===
==================================


==================================
INTRODUCTION
==================================

This project is an implementation of the abapa version of the Oware game in console mode.
When the program is executed, the user will see a menu with options to play agains another player ('Player Vs. Player'), a bot ('Player Vs. AI') and to quit the game. The player chooses an option by inputing the number that identifies the desired option.



==================================
RULES
==================================

The game follows the abapa version of Oware.
The game starts with 4 seeds in each of the 12 pits. The objective is to capture at least 25 seeds. If both players capture 24 pits, the game ends in a draw.

The players take turns playing. Player 1 always makes the first move.

On each turn, the player has three choices: 
    - surrendering (which automatically gives the win to the opponent and ends the game); 
    - claim that the game has been reduced to an endless cycle (the opponent will be prompted to confirm that this is actualy the case);
    - choose one of the 6 pits in their side of the board to sow (must be a valid move).
    
When sowing, all seeds are removed from the chosen pit and redistributed in counterclockwise direction, one in each pit. The chosen pit is always left empty; if the sowing process takes a full lap, that pit is skipped.

After sowing, if the last sowed pit is capturable, the player captures that pit, as well as all consecutive pits (clockwise direction) that are also capturable (stopping at the first pit that is not capturable, which is not captured). An exception is when the player would capture all seeds in opponent zone: in that case, no capture happens after sowing and the turn ends. 

A pit is capturable if it is in the opponent zone and contains two or three seeds after sowing. All captured seeds are removed from respective pits and added to the score of the player who did the capture. 

A legal move is a move where the chosen pit to sow is in the player's side of the board, it is non-empty and, in case the opponent's side of the board has no seeds, such move must put seeds on that side of the board. When no move is legal, the current player captures all seeds in their territory and the game ends.

When both players agree that the game has been reduced to an endless cycle (which can only be claimed when both players have seeds in their holes), each player capture the seeds in the respective side of the board, ending the game.

When the game ends, a gameover screen is shown for 4 seconds, declaring the winner and the score of each player. After that, the user returns to the main menu.



==================================
CONTROLS
==================================

In a 'Player Vs. Player' game, there will be two users controlling the keyboard. In order to avoid confusions where one player accidentaly plays on the opponent's turn, Player 1 and Player 2 have different controls for every action.
However, having to memorize two different sets of commands, one for each player, would not be desirable.

The chosen compromise is distinguishing each players' commands by case. This means that all controls are case-sensitive. Player 1 has the lower-case controls, while Player 2 has their upper-case counterpart. The controls are as follow:
  - each pit is identified by a letter (from `a` to `f` in Player 1 zone; from `A` to `F` in Player 2 zone). To choose a pit in a given turn, the player inputs the corresponding letter.
  - a player may surrender (and automatically lose the game) by inputing `q`/`Q` at the start of their turn.
  - if the game is reduced to an endless cycle, the player may vote for its end by inputing `p`/`P` at the start of their turn.
  - when prompted with a 'yes/no' question, the player may answer by inputing `y`/`Y` (answer 'yes') or `n`/`N` (answer 'no').

Encountering an EOF in stdin will be interpreted differently according to context:
  - in main menu, it is interpreted as quitting the game.
  - when choosing a move in a given turn, it is interpreted as surrendering (note that in this case there will be no confirmation window, in contrast to using `q`/`Q`).
  - when answering to a 'yes/no' prompt, it is interpreted as answering `n`/`N`.



==================================
Vs. Computer
==================================

The user may play against the computer in 'Player Vs. AI' mode. In this mode, the user controls Player 1 and the computer controls Player 2.

When player, the bot will always try to make a move. If possible, it will always choose a move that results in immediate win (25 or more seeds captured). If possible, it will always avoid moves that result in immediate lose (that give the opponent a winning move next turn).

When choosing the best move, the bot simulates all legal moves in that turn and calculates the score it ends up with choosing that move (`new_score`), as well as the best score the opponent could get in the next turn (`opponent_new_score`). It compares those results with the current score (`old_score` and `opponent_old_score`) and calculates a new number that measures how good that move is. The chosen move will be the one with the biggest such number.

If the user claims that the game has been reduced to an endless cycle, the computer will just believe the player and agree to end the game in a draw.


=== Calculating how good a move is

The final formula is as follows:
`[new_score*(new_score - old_score) - opponent_new_score*(opponent_new_score - opponent_old_score) + 625]*100 + RAND`
where `RAND` is a random number between 0 and 200. This formula seems very complex at a first glance, but it is easy to understand after explaining how I got to this formula.

I started with a very intuitive formula that would calculate the difference between the change in the player score and the change in the opponent score:
`(new_score - old_score) - (opponent_new_score - opponent_old_score)`
It seems logical that the bot would desire a move that would increase its score, but avoid increasing the score of the opponent.

However, since the objective is to be the first to get 25 seeds, it makes sense to play more agressively if we have much higher score that the opponent, while playing more defensively when we have much lower. For example, if the bot has a score of 20 and the opponent has a score of 5, it makes much more sense for the bot to try to capture seeds, even if that allows the opponent to capture seeds as well. However, if we use the simple formula, a move that would allow both players to capture 4 seeds (which would make the scores 24 and 9, respectively) would have the same payoff as a move without captures for any side.

With that in mind, a possible solution is to make make the score after move of the player weight in the importance of its score difference:
`new_score*(new_score - old_score) - opponent_new_score*(opponent_new_score - opponent_old_score)`
Similar scores will work more or less the same as the old formula, but different scores will promote a more defensive or agressive playstyle.

It is convenient in the implementation for the payoff to be always positive (so negative numbers may represent special cases that are worse than any conceivable move, such as to surrender). This formula never falls below -625 (`-25*25`), even in worst case scenarios. Because of this, the formula is shifted to positive numbers by adding a constant of 625, an operation that doesn't change the relative 'ranking' of the moves.

However, sometimes moves have identical outcomes, or outcomes so similar that it is not worth choosing one as better than the other. In that cases, a random factor comes in handy to choose one of the moves arbitrarily. Because of this, a random number between 0 and 200 is added to the formula (the range is big so it is relatively uncommon to get the same number in two moves). The previous formula is then scaled by a factor of 100, so it is not heavily influenced by this random number.

Because of this, the formula is as follows, as written in the beggining of this section:
`[new_score*(new_score - old_score) - opponent_new_score*(opponent_new_score - opponent_old_score) + 625]*100 + RAND`



==================================
State of the project
==================================

The project includes adapted parts of the code provided by the teacher on moodle which facilitates printing colored characters and clearing the console in windows. All that code is contained in the files `cmd_utils.cpp` and `cmd_utils.h`. 

All intended features have been incorporated into the project, namely:
    - the base game
    - robust input processing (invalid inputs don't make the program stop working)
    - colorful interface
    - 'Player Vs. AI' mode
    - an algorithm for the bot to pick the move with the highest immediate payoff
    - a main menu to choose the game mode and to play multiple matches without having to restart the program
