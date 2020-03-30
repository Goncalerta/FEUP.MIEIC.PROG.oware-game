#include "player.h"

Player PitOwner(int pit) {
    return pit < 6? PlayerOne : PlayerTwo;
}

Player Opponent(Player p) {
    return p == PlayerOne? PlayerTwo : PlayerOne;
}

Range PlayerZone(Player p) {
    int begin = (p == PlayerOne? 0 : 6);
    int end = begin + 5;
    
    return Range(begin, end);
}
