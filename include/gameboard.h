#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "player.h"
#include "range.h"

typedef void (*SetPitAnimator)(int, int);
typedef void (*SetScoreAnimator)(Player, int, int);

enum PitSowableState {
    ValidPit,
    PitInOpponentZone,
    EmptyPit,
    OpponentOutOfSeeds,
};

struct Gameboard {
    int p1_score;
    int p2_score;
    int pits[12];

    Gameboard();

    int PlayerScore(Player p);

    bool HasSeeds(Player p);
    bool HasLegalMove(Player p);

    PitSowableState Sowable(Player p, int pit);

    // Returns the last pit sowed.
    int Sow(int pit, SetPitAnimator set_pit);

    bool IsCapturable(Player p, int pit);
    bool IsCapturable(Range capturable_range, int pit);

    Range CaptureRange(Player p, int pit);

    int CaptureScore(Range r);
    void Capture(Player p, Range r, SetScoreAnimator set_score);

    bool IsGrandSlam(Player p, Range r);
};

#endif