#ifndef RANGE_H
#define RANGE_H

struct Range {
    int begin; // The first value of the range, inclusive. -1 if invalid range
    int end; // The last value of the range, inclusive

    // Constructs a range with the given begin and end
    // The constructed range will always be assumed to be valid, unless begin == -1
    Range(int begin, int end);

    // Returns whether this Range is valid (invalid ranges must have begin == -1)
    bool IsValid();
};

typedef struct Range CaptureRange;

#endif