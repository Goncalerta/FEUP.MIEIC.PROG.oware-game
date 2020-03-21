#include "range.h"

Range::Range(int begin, int end): begin(begin), end(end) {}
bool Range::IsValid() {
    return begin != -1;
}
