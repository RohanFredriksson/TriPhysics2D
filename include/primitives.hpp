#pragma once

#include <glm/glm.hpp>

using glm::vec2;

struct Line {
    vec2 to;
    vec2 from;
};
typedef struct Line Line;