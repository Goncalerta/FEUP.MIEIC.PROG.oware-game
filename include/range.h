#ifndef RANGE_H
#define RANGE_H

struct Range {
    int begin; // The first value of the range, inclusive
    int end; // The last value of the range, inclusive

    // Constructs a range with the given begin and end
    // The constructed range will always be assumed to be valid
    Range(int begin, int end);

    bool Contains(int n);

    bool operator==(Range const &r);
};

#endif