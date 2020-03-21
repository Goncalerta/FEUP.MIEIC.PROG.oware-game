#include "player.h"

Player PitZone(int pit) {
    return pit < 6? PlayerOne : PlayerTwo;
}

Player Opponent(Player p) {
    return p == PlayerOne? PlayerTwo : PlayerOne;
}
