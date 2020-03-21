#ifndef GAMEBOARD_H
#define GAMEBOARD_H

struct Gameboard;

#include "player.h"
#include "range.h"

struct Gameboard {
    int p1_score;
    int p2_score;
    int pits[12];

    Gameboard();

    bool HasSeeds(Player p);

    bool IsSowable(Player p, int pit);

    CaptureRange Sow(int pit);

    // TODO board has info of current player. This fucntion should either
    // be in a struct without that info or not receive Player p.
    bool IsCapturable(Player p, CaptureRange r);

    // make sure the player doesn't capture himself...
    void Capture(Player p, CaptureRange r);
};

#endif