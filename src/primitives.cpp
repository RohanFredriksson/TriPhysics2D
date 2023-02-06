#include <cmath>
#include "primitives.hpp"

void rotateVector(vec2& vec, float degrees, vec2 origin) {

    float x = vec.x - origin.x;
    float y = vec.y - origin.y;
    float radians = degrees * M_PI / 180.0f;

    vec.x = origin.x + ((x * (float) cos(radians)) - (y * (float) sin(radians)));
    vec.y = origin.y + ((x * (float) sin(radians)) + (y * (float) cos(radians)));

}

Shape::Shape(float rotation) {
    this->rotation = rotation;
}

Triangle::Triangle(vec2 a, vec2 b, vec2 c, float rotation) : Shape::Shape(rotation) {
    this->a = a;
    this->b = b;
    this->c = c;
}

Triangle::Triangle(vec2 a, vec2 b, vec2 c) : Shape::Shape(0.0f) {
    this->a = a;
    this->b = b;
    this->c = c;
}

vec2 Triangle::centroid() {
    return vec2((this->a.x + this->b.x + this->c.x) / 3.0f, (this->a.y + this->b.y + this->c.y) / 3.0f);
}

void Triangle::rotate(float degrees, vec2 origin) {
    rotateVector(this->a, degrees, origin);
    rotateVector(this->b, degrees, origin);
    rotateVector(this->b, degrees, origin);
    this->rotation += degrees;
}

void Triangle::translate(vec2 by) {
    this->a += by;
    this->b += by;
    this->c += by;
}

/*
Line::Line(vec2 from, vec2 to) {
    this->from = from;
    this->to = to;
}

vec2 Line::getMin() {
    return vec2(std::min(this->from.x, this->to.x), std::min(this->from.y, this->to.y));
}

vec2 Line::getMax() {
    return vec2(std::max(this->from.x, this->to.x), std::max(this->from.y, this->to.y));
}

float Line::getGradient() {
    return (this->to.y - this->from.y) / (this->to.x - this->from.x);
}

float Line::getIntercept() {
    return this->from.y - this->getGradient() * this->from.x;
}

bool Line::isVertical() {
    return this->from.x == this->to.x;
}
*/