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

vec2 Triangle::centroid() {
    return vec2((this->a.x + this->b.x + this->c.x) / 3.0f, (this->a.y + this->b.y + this->c.y) / 3.0f);
}

Line Triangle::left(vec2 vertex) {
    if (vertex == this->a) {return Line(this->a, this->c);}
    if (vertex == this->b) {return Line(this->b, this->a);}
    if (vertex == this->c) {return Line(this->c, this->b);}
    return Line(vec2(0.0f, 0.0f), vec2(0.0f, 0.0f));
}

Line Triangle::right(vec2 vertex) {
    if (vertex == this->a) {return Line(this->a, this->b);}
    if (vertex == this->b) {return Line(this->b, this->c);}
    if (vertex == this->c) {return Line(this->c, this->a);}
    return Line(vec2(0.0f, 0.0f), vec2(0.0f, 0.0f));
}

Circle::Circle(float radius, vec2 centre) {
    this->radius = radius;
    this->centre = centre;
}

void Circle::translate(vec2 by) {
    this->centre += by;
}