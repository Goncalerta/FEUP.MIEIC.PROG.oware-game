#include "range.h"

Range::Range(int begin, int end): begin(begin), end(end) {}

bool Range::Contains(int n) {
    return n >= begin && n <= end;
}

bool Range::operator==(Range const &r) { 
    return begin == r.begin && end == r.end;
} 
