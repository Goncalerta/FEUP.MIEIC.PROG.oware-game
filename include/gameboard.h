#ifndef GAMEBOARD_H
#define GAMEBOARD_H

struct Gameboard;
enum PitSowableState {
    ValidPit,
    PitInOpponentZone,
    EmptyPit,
    OpponentOutOfSeeds,
};

#include "player.h"
#include "range.h"
#include "cmd_handle.h"

struct Gameboard {
    int p1_score;
    int p2_score;
    int pits[12];

    Gameboard();

    Range PlayerBoard(Player p);

    bool HasSeeds(Player p);

    PitSowableState Sowable(Player p, int pit);

    // Returns the last pit sowed.
    int Sow(int pit, CmdHandle &handle);

    bool IsCapturable(Player p, int pit);
    bool IsCapturable(Range playerboard, int pit);

    Range CaptureRange(Player p, int pit);

    // make sure the player doesn't capture himself...
    void Capture(Player p, Range r);

    bool IsGrandSlam(Player p, Range r);
};

#endif