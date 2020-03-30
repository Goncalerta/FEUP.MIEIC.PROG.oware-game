#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "player.h"
#include "range.h"

// A callback function to be used by `Gameboard::Sow` to animate 
// updating pit values.
//
// Parameter 1: The pit (a number between 0 and 11) being set.
// Parameter 2: The new number of seeds in that pit.
typedef void (*SetPitAnimator)(int, int);

// A callback function to be used by `Gameboard::Capture` to animate 
// updating score values.
//
// Parameter 1: The player whose score is being updated.
// Parameter 2: The old score of that player.
// Parameter 3: The new score of that player.
typedef void (*SetScoreAnimator)(Player, int, int);

// The return value of `Gameboard::Sowable`.
//
// Represents either that the pit may be sowed successfully 
// (`ValidPit`) or the reason because of which it may not.
enum PitSowableState {
    // The pit may be sowed successfully.
    ValidPit,
    // Player doesn't own that pit.
    PitInOpponentZone,
    // Pit has 0 seeds.
    EmptyPit,
    // Player must give seeds to the opponent and this pit 
    // fails to do so.
    OpponentOutOfSeeds,
};

// The oware `Gameboard`, containing the pits and score of each Player. 
// This is where most of the gameplay logic is controlled.
struct Gameboard {
    // Current score of Player 1.
    int p1_score;
    // Current score of Player 2.
    int p2_score;
    // Current state of the 12 pits in the game.
    int pits[12];

    // Constructs a new gameboard with 4 seeds in every pit and 0
    // score for both players.
    Gameboard();

    // Returns the current score of the given Player `p`.
    int PlayerScore(Player p);

    // Returns whether the given Player `p` owns any non-empty pit.
    bool HasSeeds(Player p);

    // Returns whether the given Player `p` may sow at least one pit.
    bool HasLegalMove(Player p);

    // Returns either that the given `pit` (a number between 0 and 11)
    // may be sowed successfully by Player `p`, or the reason because 
    // of which it may not.
    PitSowableState Sowable(Player p, int pit);

    // Sows the given `pit` (a number between 0 and 11), distributing
    // all of its seeds, one by one counterclockwise, by the remaining 
    // pits.
    // A callback `set_pit` may be passed, which should animate the
    // updating of each pit's number of seeds while sowing.
    //
    // Returns the last pit sowed.
    int Sow(int pit, SetPitAnimator set_pit);

    // Returns whether the given `pit` (a number between 0 and 11) may
    // be captured by Player `p`.
    bool IsCapturable(Player p, int pit);

    // Returns whether the given `pit` (a number between 0 and 11) may 
    // be captured. Only pits in the given `capturable_range` may be captured.
    //
    // This overload allows the optimization of `Gameboard::IsCapturable(Player, int)`
    // when it is called multiple times, as it avoids to recalculating the 
    // `capturable_range` over and over.
    bool IsCapturable(Range capturable_range, int pit);

    // Returns the `Range` of pits that would be captured by Player `p`
    // if the last pit sowed was `pit`.
    //
    // Parameter `pit` must valid (a number between 0 and 11) and
    // capturable (`Gameboard::IsCapturable(p, pit)` must return true).
    Range CaptureRange(Player p, int pit);

    // `Player p` captures the given `Range r`, scoring all seeds in 
    // that `Range`.
    // A callback `set_score` may be passed, which should animate the
    // score updating.
    void Capture(Player p, Range r, SetScoreAnimator set_score);

    // Returns whether capturing all pits in `Range r` by `Player p` would
    // be considered a 'GrandSlam'.
    //
    // According to the rules of Oware, a GrandSlam is when a player
    // would capture every seed in the opponent pit. In abapa version,
    // GrandSlams are unallowed. While the move that would result in
    // a GrandSlame is legal, no capture results of it.
    bool IsGrandSlam(Player p, Range r);
};

#endif
