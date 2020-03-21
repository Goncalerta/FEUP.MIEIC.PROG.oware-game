#include <iostream>
#include "gameboard.h"

Gameboard::Gameboard(): p1_score(0), p2_score(0) {
    for(int i = 0; i < 12; i++) {
        pits[i] = 4;
    }
}

bool Gameboard::IsOver() {
    return p1_score == 25 || p2_score == 25 || (p1_score == 24 && p2_score == 24);
}

bool Gameboard::HasSeeds(Player p) {
    int start = p == PlayerOne? 0 : 6;
    for(int i = start; i < start+6; i++) {
        if(pits[i] != 0) return true;
    }
    return false;
}

bool Gameboard::IsSowable(Player p, int pit) {
    bool in_player_zone = PitZone(pit) == p;
    bool in_enemy_zone = !in_player_zone;

    // Must be a nonempty pit in the current player zone
    if(pits[pit] == 0 || !in_player_zone) return false;
    // If the enemy has nonempty pits, all is fine
    if(HasSeeds(Opponent(p))) return true;
    // Otherwise, the player must make a move that gives
    // the opponent seeds.
    int player_zone_end_pit = p == PlayerOne? 5 : 11;
    int seeds = pits[pit];
    return player_zone_end_pit - pit + 1 < seeds;
}

CaptureRange Gameboard::Sow(int pit) {
    int seeds = pits[pit];
    pits[pit] = 0;
    int current_pit = pit;
    int capture_begin = -1; // -1 means no capture will happen
    while(seeds != 0) {
        current_pit++;
        if(current_pit == 12) current_pit = 0;
        if(current_pit == pit) continue;

        pits[current_pit]++;
        seeds--;
        if(pits[current_pit] == 2 || pits[current_pit] == 3) {
            if(capture_begin == -1) capture_begin = current_pit;
        } else {
            capture_begin = -1;
        }
    }
    int capture_end = current_pit;

    // TODO trim capture range to enemy field
    return CaptureRange(capture_begin, capture_end);
}

// TODO board has info of current player. This fucntion should either
// be in a struct without that info or not receive Player p.
bool Gameboard::IsCapturable(Player p, CaptureRange r) {
    if(!r.IsValid()) return false;
    int i = p == PlayerOne? 6 : 0;
    int end = i + 6;
    while(i < end) {
        if(i == r.begin) {
            i = r.end + 1;
        } else {
            if(pits[i] != 0) return true;
            i++;
        }
    }
    return false;
}

// make sure the player doesn't capture himself...
void Gameboard::Capture(Player p, CaptureRange r) {
    int score;

    for(int i = r.begin; i <= r.end; i++) {
        score += pits[i];
        pits[i] = 0;
    }

    if(p == PlayerOne) {
        p1_score += score;
    } else {
        p1_score = score;
    }
}

void Gameboard::Draw() {
    std::cout << "              F  E  D  C  B  A" << std::endl;
    std::cout << "Player Two    ";
    for(int i = 11; i >= 6; i--) {
        std::cout << pits[i] << "  ";
    }
    std::cout << "   Score: " << p2_score;
    std::cout << std::endl << "         -----------------------------" << std::endl;
    std::cout << "Player One    ";
    for(int i = 0; i <= 5; i++) {
        std::cout << pits[i] << "  ";
    }
    std::cout << "   Score: " << p1_score;
    std::cout << std::endl << "              a  b  c  d  e  f" << std::endl;
}
