#pragma once

#include <glm/glm.hpp>

using glm::vec2;

class Shape {

    public:
        
        float rotation;

        Shape(float rotation);
        virtual vec2 centroid();
        virtual void rotate(float degrees, vec2 origin);
        virtual void translate(vec2 by);

};

class Triangle : public Shape {

    public:

        vec2 a;
        vec2 b;
        vec2 c;
    
        Triangle(vec2 a, vec2 b, vec2 c, float rotation);
        Triangle(vec2 a, vec2 b, vec2 c);
        vec2 centroid() override;
        void rotate(float degrees, vec2 origin) override;
        void translate(vec2 by) override;

};