#ifndef PLAYER_H
#define PLAYER_H

#include "range.h"

// Represents a player, either Player 1 or Player 2.
enum Player{
    PlayerOne, PlayerTwo
};

// Returns the `Player` that owns the given `pit`.
//
// `pit` must represent a valid pit (a number between 0 and 11).
Player PitOwner(int pit);

// Returns the opponent of the given `Player p`.
// 
// That is:
// -  Opponent(PlayerOne) == PlayerTwo
// -  Opponent(PlayerTwo) == PlayerOne
Player Opponent(Player p);

// Returns the range of pits that belong to the given `Player p`.
Range PlayerZone(Player p);

#endif
