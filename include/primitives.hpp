#pragma once

#include <glm/glm.hpp>

using glm::vec2;

class Line {

    public:
        
        vec2 start;
        vec2 end;

        Line(vec2 start, vec2 end);
        void rotate(float degrees, vec2 origin);
        void translate(vec2 by);

        vec2 getMin();
        vec2 getMax();
        float getGradient();
        float getIntercept();
        bool isVertical();

};

class Triangle {

    public:

        vec2 a;
        vec2 b;
        vec2 c;

        Triangle(vec2 a, vec2 b, vec2 c);
        void rotate(float degrees, vec2 origin);
        void translate(vec2 by);

};

class Circle {

    public:

        vec2 centre;
        float radius;

        Circle(vec2 centre, float radius);
        void rotate(float degrees, vec2 origin);
        void translate(vec2 by);

};