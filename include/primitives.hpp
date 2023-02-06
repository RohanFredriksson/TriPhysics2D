#pragma once

#include <glm/glm.hpp>

using glm::vec2;

class Shape {

    public:
        
        float rotation;
        vec2 position;

        virtual void rotate(float degrees, vec2 origin);
        void translate(vec2 by);

};

class Triangle : public Shape {

    public:

        vec2 p2a;
        vec2 p2b;
        vec2 p2c;

        Triangle(vec2 position, vec2 p2a, vec2 p2b, vec2 p2c, float rotation);
        Triangle(vec2 position, vec2 p2a, vec2 p2b, vec2 p2c);
        Triangle(vec2 a, vec2 b, vec2 c, float rotation);
        Triangle(vec2 a, vec2 b, vec2 c);
        
        void rotate(float degrees, vec2 origin) override;
        
        vec2 a();
        vec2 b();
        vec2 c();

};

class Circle : public Shape {

    public:

        float radius;

        Circle(vec2 position, float radius, float rotation);
        Circle(vec2 position, float radius);

        void rotate(float degrees, vec2 origin) override;

};