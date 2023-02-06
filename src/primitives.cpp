#include <algorithm>
#include "primitives.hpp"

Line::Line(vec2 from, vec2 to) {
    this->set(from, to);
}

vec2 Line::getFrom() {
    return this->from;
}

vec2 Line::getTo() {
    return this->to;
}

vec2 Line::getMin() {
    return vec2(std::min(this->from.x, this->to.x), std::min(this->from.y, this->to.y));
}

vec2 Line::getMax() {
    return vec2(std::max(this->from.x, this->to.x), std::max(this->from.y, this->to.y));
}

float Line::getGradient() {
    return this->gradient;
}

float Line::getIntercept() {
    return this->from.y - this->gradient * this->from.x;
}

bool Line::isVertical() {
    return this->from.x == this->to.x;
}

bool Line::isHorizontal() {
    return this->from.y == this->to.y;
}

void Line::setFrom(vec2 from) {
    this->set(from, this->to);
}

void Line::setTo(vec2 to) {
    this->set(this->from, to);
}

void Line::set(vec2 from, vec2 to) {
    this->from = from;
    this->to = to;
    this->gradient = (to.y - from.y) / (to.x - from.x);
}