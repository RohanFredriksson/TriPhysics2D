#include <cmath>
#include "primitives.hpp"

void rotateVector(vec2& vec, float degrees, vec2 origin) {

    float x = vec.x - origin.x;
    float y = vec.y - origin.y;
    float radians = degrees * M_PI / 180.0f;

    vec.x = origin.x + ((x * (float) cos(radians)) - (y * (float) sin(radians)));
    vec.y = origin.y + ((x * (float) sin(radians)) + (y * (float) cos(radians)));

}

Body::Body() {

    this->collider = nullptr;
    this->force = vec2(0.0f, 0.0f);
    this->velocity = vec2(0.0f, 0.0f);

    this->position = vec2(0.0f, 0.0f);
    this->rotation = 0.0f;

    this->mass = 0.0f;
    this->restitution = 1.0f;
    this->friction = 1.0f;
    this->sensor = false;

}

void Body::rotate(float degrees) {
    if (this->collider != nullptr) {this->collider->rotate(degrees);}
    this->rotation += degrees;
}

void Body::translate(vec2 vector) {
    this->position += vector;
}

void Shape::rotate(float degrees) {

}

Triangle::Triangle(vec2 p2a, vec2 p2b, vec2 p2c) {

    this->p2a = p2a;
    this->p2b = p2b;
    this->p2c = p2c;

}

void Triangle::rotate(float degrees) {

    vec2 origin = vec2(0.0f, 0.0f);
    rotateVector(this->p2a, degrees, origin);
    rotateVector(this->p2b, degrees, origin);
    rotateVector(this->p2c, degrees, origin);
    
}

vec2 Triangle::a() {
    if (this->body == nullptr) {return this->p2a;}
    return this->body->position + this->p2a;
}

vec2 Triangle::b() {
    if (this->body == nullptr) {return this->p2b;}
    return this->body->position + this->p2b;
}

vec2 Triangle::c() {
    if (this->body == nullptr) {return this->p2c;}
    return this->body->position + this->p2c;
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

Circle::Circle(float radius) {
    this->radius = radius;
}

void Circle::rotate(float degrees)  {
    
}