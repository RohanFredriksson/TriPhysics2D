#pragma once

#include "primitives.hpp"

struct IntersectionResult {
    bool intersecting;
    vec2 point;
};
typedef struct IntersectionResult IntersectionResult;

IntersectionResult isIntersecting(Line a, Line b);