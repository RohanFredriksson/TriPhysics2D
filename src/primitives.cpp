#include <cmath>
#include <algorithm>
#include "primitives.hpp"

namespace {

    void rotateVector(vec2& vec, float degrees, vec2 origin) {

        float x = vec.x - origin.x;
        float y = vec.y - origin.y;
        float radians = degrees * M_PI / 180.0f;

        vec.x = origin.x + ((x * (float) cos(radians)) - (y * (float) sin(radians)));
        vec.y = origin.y + ((x * (float) sin(radians)) + (y * (float) cos(radians)));

    }

}

Line::Line(vec2 start, vec2 end) {
    this->start = start;
    this->end = end;
}

void Line::rotate(float degrees, vec2 origin) {
    rotateVector(this->start, degrees, origin);
    rotateVector(this->end, degrees, origin);
}

void Line::translate(vec2 by) {
    this->start += by;
    this->end += by;
}

vec2 Line::getMin() {
    return vec2(std::min(this->start.x, this->end.x), std::min(this->start.y, this->end.y));
}

vec2 Line::getMax() {
    return vec2(std::max(this->start.x, this->end.x), std::max(this->start.y, this->end.y));
}

float Line::getGradient() {
    return (this->end.y - this->start.y) / (this->end.x - this->start.x);
}

float Line::getIntercept() {
    return this->start.y - this->getGradient() * this->start.x;
}

bool Line::isVertical() {
    return this->start.x == this->end.x;
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

Circle::Circle(vec2 centre, float radius) {
    this->centre = centre;
    this->radius = radius;
}

void Circle::rotate(float degrees, vec2 origin)  {
    rotateVector(this->centre, degrees, origin);
}

void Circle::translate(vec2 by) {
    this->centre += by;
}