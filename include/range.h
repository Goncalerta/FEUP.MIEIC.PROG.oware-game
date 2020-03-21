#ifndef RANGE_H
#define RANGE_H

typedef struct Range {
    int begin; // The first value of the range, inclusive. -1 if invalid range
    int end; // The last value of the range, inclusive

    Range(int begin, int end);
    bool IsValid();
} Range;

typedef struct Range CaptureRange;

#endif