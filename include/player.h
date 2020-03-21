#ifndef PLAYER_H
#define PLAYER_H

enum Player{
    PlayerOne, PlayerTwo
};

Player PitZone(int pit);
Player Opponent(Player p);

#endif