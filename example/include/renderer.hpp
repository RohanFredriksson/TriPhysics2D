#pragma once

#include <glm/glm.hpp>
#include <trip2d/trip2d.hpp>

using glm::vec2;
using glm::vec3;

namespace Renderer {

    void init();
    void render();
    void destroy();

    void drawLine(vec2 from, vec2 to, vec3 colour, int lifetime);
    void drawBox(vec2 centre, vec2 dimensions, float rotation, vec3 colour, int lifetime);
    void drawCircle(vec2 centre, float radius, vec3 colour, int lifetime);
    void drawTriangle(vec2 a, vec2 b, vec2 c, vec3 colour, int lifetime);
    void drawShape(Shape* shape, vec3 colour, int lifetime);

}