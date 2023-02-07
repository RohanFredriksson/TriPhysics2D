#include "force.hpp"

ForceRegistration::ForceRegistration(ForceGenerator* generator, Body* body) {
    this->generator = generator;
    this->body = body;
}

void ForceRegistry::add(ForceGenerator* generator, Body* body) {
    ForceRegistration registration = ForceRegistration(generator, body);
    this->registry.push_back(registration);
}

void ForceRegistry::remove(ForceGenerator* generator, Body* body) {
    int n = this->registry.size();
    for (int i = 0; i < n; i++) {
        ForceRegistration registration = this->registry[i];
        if (registration.generator == generator && registration.body == body) {
            this->registry.erase(this->registry.begin() + i);
            return;
        }
    }
}

void ForceRegistry::clear() {
    this->registry.clear();
}

void ForceRegistry::updateForces(float dt) {
    int n = this->registry.size();
    for (int i = 0; i < n; i++) {
        ForceRegistration registration = this->registry[i];
        registration.generator->updateForce(registration.body, dt);
    }
}

void ForceRegistry::zeroForces() {
    int n = this->registry.size();
    for (int i = 0; i < n; i++) {
        ForceRegistration registration = this->registry[i];
        registration.body->force = vec2(0.0f, 0.0f);
    }
}

Gravity::Gravity(vec2 gravity) {
    this->gravity = gravity;
}

void Gravity::updateForce(Body* body, float dt)  {
    vec2 acceleration = this->gravity * body->mass;
    body->force += acceleration;
}

vec2 Gravity::getGravity() {
    return this->gravity;
}

void Gravity::setGravity(vec2 gravity)  {
    this->gravity = gravity;
}