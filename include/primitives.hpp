#pragma once

#include <glm/glm.hpp>

using glm::vec2;

class Line {

    private:
        vec2 from;
        vec2 to;
        float gradient;

    public:

        Line(vec2 from, vec2 to);

        vec2 getFrom();
        vec2 getTo();
        vec2 getMin();
        vec2 getMax();
        float getGradient();
        float getIntercept();
        bool isVertical();
        bool isHorizontal();

        void setFrom(vec2 from);
        void setTo(vec2 to);
        void set(vec2 from, vec2 to);

};