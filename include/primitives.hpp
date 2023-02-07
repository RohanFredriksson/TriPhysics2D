#pragma once

#include <glm/glm.hpp>

using glm::vec2;

class Shape;

class Body {

    public:

        Shape* collider;
        vec2 force;
        vec2 velocity;

        vec2 position;
        float rotation;
        
        float mass;
        float restitution;
        float friction;
        bool sensor;

        Body();
        void rotate(float degrees);
        void translate(vec2 vector);

};

class Shape {

    public:
        
        Body* body;
        virtual void rotate(float degrees);

};

class Triangle : public Shape {

    public:

        vec2 p2a;
        vec2 p2b;
        vec2 p2c;

        Triangle(vec2 p2a, vec2 p2b, vec2 p2c);
        void rotate(float degrees) override;
        
        vec2 a();
        vec2 b();
        vec2 c();

        vec2 a2b();
        vec2 a2c();
        vec2 b2c();

};

class Circle : public Shape {

    public:

        float radius;

        Circle(float radius);
        void rotate(float degrees) override;

};