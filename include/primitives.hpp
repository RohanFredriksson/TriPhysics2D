#pragma once

#include <glm/glm.hpp>

using glm::vec2;

void rotateVector(vec2& vec, float degrees, vec2 origin);

class Triangle {

    public:

        vec2 a;
        vec2 b;
        vec2 c;

        Triangle(vec2 a, vec2 b, vec2 c);
        void rotate(float degrees, vec2 origin);
        void translate(vec2 by);

        vec2 getCentroid();

};

class Circle {

    public:

        float radius;
        vec2 centre;

        Circle(float radius, vec2 centre);
        void rotate(float degrees, vec2 origin);
        void translate(vec2 by);

};