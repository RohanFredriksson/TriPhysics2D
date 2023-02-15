#include <cmath>
#include <algorithm>
#include "primitives.hpp"

void rotateVector(vec2& vec, float degrees, vec2 origin) {

    float x = vec.x - origin.x;
    float y = vec.y - origin.y;
    float radians = degrees * M_PI / 180.0f;

    vec.x = origin.x + ((x * (float) cos(radians)) - (y * (float) sin(radians)));
    vec.y = origin.y + ((x * (float) sin(radians)) + (y * (float) cos(radians)));

}

void Shape::rotate(float degrees, vec2 origin) {

}

void Shape::translate(vec2 by) {
    
}

Triangle::Triangle(vec2 a, vec2 b, vec2 c) {
    this->a = a;
    this->b = b;
    this->c = c;
}

void Triangle::rotate(float degrees, vec2 origin) {
    rotateVector(this->a, degrees, origin);
    rotateVector(this->b, degrees, origin);
    rotateVector(this->c, degrees, origin);
}

void Triangle::translate(vec2 by) {
    this->a += by;
    this->b += by;
    this->c += by;
}

vec2 Triangle::getCentroid() {
    return vec2((this->a.x + this->b.x + this->c.x) / 3.0f, (this->a.y + this->b.y + this->c.y) / 3.0f);
}

Circle::Circle(float radius, vec2 centre) {
    this->radius = radius;
    this->centre = centre;
}

void Circle::translate(vec2 by) {
    this->centre += by;
}