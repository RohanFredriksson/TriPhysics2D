#include <cmath>
#include "primitives.hpp"

void rotateVector(vec2& vec, float degrees, vec2 origin) {

    float x = vec.x - origin.x;
    float y = vec.y - origin.y;
    float radians = degrees * M_PI / 180.0f;

    vec.x = origin.x + ((x * (float) cos(radians)) - (y * (float) sin(radians)));
    vec.y = origin.y + ((x * (float) sin(radians)) + (y * (float) cos(radians)));

}

void Shape::translate(vec2 by) {
    this->position += by;
}

void Shape::rotate(float degrees, vec2 origin) {
    rotateVector(this->position, degrees, origin);
    this->rotation += degrees;
}

Triangle::Triangle(vec2 position, vec2 p2a, vec2 p2b, vec2 p2c, float rotation) {

    this->position = position;
    this->rotation = rotation;

    this->p2a = p2a;
    this->p2b = p2b;
    this->p2c = p2c;

}

Triangle::Triangle(vec2 position, vec2 p2a, vec2 p2b, vec2 p2c) {

    this->position = position;
    this->rotation = 0.0f;

    this->p2a = p2a;
    this->p2b = p2b;
    this->p2c = p2c;

}

Triangle::Triangle(vec2 a, vec2 b, vec2 c, float rotation) {
    
    vec2 position = vec2((a.x + b.x + c.x) / 3.0f, (a.y + b.y + c.y) / 3.0f);
    this->position = position;
    this->rotation = rotation;
    
    this->p2a = a - position;
    this->p2b = b - position;
    this->p2c = c - position;

}

Triangle::Triangle(vec2 a, vec2 b, vec2 c) {

    vec2 position = vec2((a.x + b.x + c.x) / 3.0f, (a.y + b.y + c.y) / 3.0f);
    this->position = position;
    this->rotation = 0.0f;
    
    this->p2a = a - position;
    this->p2b = b - position;
    this->p2c = c - position;

}

void Triangle::rotate(float degrees, vec2 origin) {

    rotateVector(this->p2a, degrees, this->position);
    rotateVector(this->p2b, degrees, this->position);
    rotateVector(this->p2c, degrees, this->position);

    Shape::rotate(degrees, origin);
    
}

vec2 Triangle::a() {
    return this->position + this->p2a;
}

vec2 Triangle::b() {
    return this->position + this->p2b;
}

vec2 Triangle::c() {
    return this->position + this->p2c;
}

vec2 Triangle::a2b() {
    return this->p2b - this->p2a;
}

vec2 Triangle::a2c() {
    return this->p2c - this->p2a;
}

vec2 Triangle::b2c() {
    return this->p2c - this->p2b;
}

Circle::Circle(vec2 position, float radius, float rotation) {

    this->position = position;
    this->rotation = rotation;

    this->radius = radius;

}

Circle::Circle(vec2 position, float radius) {

    this->position = position;
    this->rotation = 0.0f;

    this->radius = radius;

}

void Circle::rotate(float degrees, vec2 origin)  {
    Shape::rotate(degrees, origin);
}