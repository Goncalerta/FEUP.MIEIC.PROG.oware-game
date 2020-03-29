#include "player.h"

Player PitZone(int pit) {
    return pit < 6? PlayerOne : PlayerTwo;
}

Player Opponent(Player p) {
    return p == PlayerOne? PlayerTwo : PlayerOne;
}

Range PlayerBoard(Player p) {
    int begin = p == PlayerOne? 0 : 6;
    int end = begin + 5;
    
    return Range(begin, end);
}
