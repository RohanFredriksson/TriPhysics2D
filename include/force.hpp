#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "primitives.hpp"

using std::vector;
using glm::vec2;

class ForceGenerator {

    public:
        virtual void updateForce(Body* body, float dt) = 0;

};

class ForceRegistry;

class ForceRegistration {
    
    public:

        ForceGenerator* generator;
        Body* body;

        ForceRegistration(ForceGenerator* generator, Body* body);

};

class ForceRegistry {

    private:
        vector<ForceRegistration> registry;

    public:

        void add(ForceGenerator* generator, Body* body);
        void remove(ForceGenerator* generator, Body* body);
        void clear();
        void updateForces(float dt);
        void zeroForces();

};

class Gravity : public ForceGenerator {

    private:

        vec2 gravity;

    public:

        Gravity(vec2 gravity);
        void updateForce(Body* body, float dt) override;

        vec2 getGravity();
        void setGravity(vec2 gravity);

};