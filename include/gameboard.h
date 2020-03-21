#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "player.h"
#include "range.h"

typedef struct Gameboard {
    int p1_score;
    int p2_score;
    // TODO must be in struct Game {}
    int pits[12];

    Gameboard();

    bool IsOver();

    bool HasSeeds(Player p);

    bool IsSowable(Player p, int pit);

    CaptureRange Sow(int pit);

    // TODO board has info of current player. This fucntion should either
    // be in a struct without that info or not receive Player p.
    bool IsCapturable(Player p, CaptureRange r);

    // make sure the player doesn't capture himself...
    void Capture(Player p, CaptureRange r);

    void Draw();
} Gameboard;

#endif