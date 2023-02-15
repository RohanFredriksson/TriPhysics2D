#pragma once

#include <glm/glm.hpp>

using glm::vec2;

void rotateVector(vec2& vec, float degrees, vec2 origin);

class Shape {

    public:

        virtual void rotate(float degrees, vec2 origin);
        virtual void translate(vec2 by);

};

class Triangle : public Shape {

    public:

        vec2 a;
        vec2 b;
        vec2 c;

        Triangle(vec2 a, vec2 b, vec2 c);
        void rotate(float degrees, vec2 origin) override;
        void translate(vec2 by) override;

        vec2 getCentroid();

};

class Circle : public Shape {

    public:

        float radius;
        vec2 centre;

        Circle(float radius, vec2 centre);
        void translate(vec2 by) override;

};