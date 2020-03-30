#ifndef RANGE_H
#define RANGE_H

// Represents a valid range of pits.
struct Range {
    // The first value of the range, inclusive
    int begin;
    // The last value of the range, inclusive
    int end; 


    // Constructs a `Range` of pits with the given `begin` and `end`.
    // The parameters must be valid, which means (0 <= `begin` <= `end` <= 11).
    Range(int begin, int end);

    // Returns whether this `Range` contains the given pit `n`.
    //
    // That is, whether `Range::begin` <= `n` <= `Range::end`.
    // `n` must represent a valid pit (a number between 0 and 11).
    bool Contains(int n);

    // Returns whether two `Range`s are the same.
    // 
    // That is, whether both have the same value of `Range::begin` and 
    // `Range::end`.
    bool operator==(Range const &r);
};

#endif
