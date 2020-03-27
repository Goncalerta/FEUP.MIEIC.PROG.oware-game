#include <iostream>
#include "gameboard.h"

Gameboard::Gameboard(): p1_score(0), p2_score(0) {
    for(int i = 0; i < 12; i++) {
        pits[i] = 4;
    }
}

int Gameboard::PlayerScore(Player p) {
    if(p == PlayerOne) {
        return p1_score;
    } else {
        return p2_score;
    }
}


bool Gameboard::HasSeeds(Player p) {
    int start = p == PlayerOne? 0 : 6;
    for(int i = start; i < start+6; i++) {
        if(pits[i] != 0) return true;
    }
    return false;
}


bool Gameboard::HasLegalMove(Player p) {
    Range player_board = PlayerBoard(p);
    bool opponent_has_seeds = HasSeeds(Opponent(p));
    
    for(int i = player_board.begin; i <= player_board.end; i++) {
        if(pits[i] == 0) continue;
        if(opponent_has_seeds || pits[i] > player_board.end - i) return true;
    }
    return false;
}


PitSowableState Gameboard::Sowable(Player p, int pit) {
    bool in_player_zone = PitZone(pit) == p;

    // Must be a nonempty pit in the current player zone
    if(!in_player_zone) return PitInOpponentZone;
    if(pits[pit] == 0) return EmptyPit;

    // If the enemy has nonempty pits, all is fine
    if(HasSeeds(Opponent(p))) return ValidPit;
    
    // Otherwise, the player must make a move that gives
    // the opponent seeds.
    int player_zone_end_pit = p == PlayerOne? 5 : 11;
    bool gives_seeds = pits[pit] > player_zone_end_pit - pit;
    if(!gives_seeds) return OpponentOutOfSeeds;

    return ValidPit;
}

int Gameboard::Sow(int pit, SetPitAnimator set_pit) {
    int current_pit = pit;
    int seeds = pits[pit];

    pits[pit] = 0;
    if(set_pit) set_pit(pit, 0);

    while(seeds != 0) {
        current_pit++;
        if(current_pit == 12) current_pit = 0;
        if(current_pit == pit) continue;

        pits[current_pit]++;
        if(set_pit) set_pit(current_pit, pits[current_pit]);

        seeds--;
    }

    return current_pit;
}

bool Gameboard::IsCapturable(Player p, int pit) {
    return IsCapturable(PlayerBoard(p), pit);
}

bool Gameboard::IsCapturable(Range player_board, int pit) {
    // A player can't capture their own board
    if(player_board.Contains(pit)) return false;
    return pits[pit] == 2 || pits[pit] == 3;
}

Range Gameboard::CaptureRange(Player p, int pit) {
    int end = pit;
    int begin = end;
    Range player_board = PlayerBoard(p);

    while(IsCapturable(player_board, begin-1)) {
        begin--;
    }

    return Range(begin, end);
}

int Gameboard::CaptureScore(Range r) {
    int score = 0;

    for(int i = r.begin; i <= r.end; i++) {
        score += pits[i];
        pits[i] = 0;
    }
    
    return score;
}

void Gameboard::Capture(Player p, Range r) {
    int score = CaptureScore(r);

    if(p == PlayerOne) {
        p1_score += score;
    } else {
        p2_score += score;
    }
}

bool Gameboard::IsGrandSlam(Player p, Range capture) {
    Range opponent_board = PlayerBoard(Opponent(p));
    for(int i = opponent_board.begin; i <= opponent_board.end; i++) {
        if(pits[i] != 0 && !capture.Contains(i)) return false;
    }
    return true;
}
