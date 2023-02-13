#include <cmath>
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include "collision.hpp"

#include <iostream>

CollisionResult getCollision(Circle a, Circle b) {

    // Determine if the two circles are colliding.
    float sumRadii = a.radius + b.radius;
    vec2 distance = b.centre - a.centre;
    if (glm::dot(distance, distance) - (sumRadii * sumRadii) > 0) {return {false, vec2(0.0f, 0.0f), 0.0f};}

    // Find the depth and normal of the collision
    float depth = fabsf(glm::length(distance) - sumRadii) * 0.5f;
    vec2 normal = glm::normalize(distance);

    // Find the contact point of the collision
    float distanceToPoint = a.radius - depth;
    vec2 contactPoint = distanceToPoint * normal + a.centre;

    return {true, contactPoint, depth};

}

CollisionResult getCollision(Circle c, Triangle t) {

    // Keep track of the translation that occurs.
    vec2 translation = vec2(0.0f, 0.0f);
    float rotation = 0.0f;

    // Find the longest side length
    vec2 ab = t.b - t.a;
    vec2 ac = t.c - t.a;
    vec2 bc = t.c - t.b;

    float ab2 = glm::dot(ab, ab);
    float ac2 = glm::dot(ac, ac);
    float bc2 = glm::dot(bc, bc);
    float longest = std::max(std::max(ab2, ac2), bc2);

    // If AB is not the longest, swap the vertices, such that AB is the longest.
    if (ac2 == longest) {
        vec2 tmp = t.b;
        t.b = t.c;
        t.c = tmp;
    }

    else if (bc2 == longest) {
        vec2 tmp = t.a;
        t.a = t.b;
        t.b = t.c;
        t.c = tmp;
    }

    // If ab is vertical, make ab horizontal.
    ab = t.b - t.a;
    if (ab.x == 0) {
        t.rotate(90.0f, vec2(0.0f, 0.0f));
        c.rotate(90.0f, vec2(0.0f, 0.0f));
        rotation += 90.0f;
    }

    // Rotate the space such that ab is horizontal
    ab = t.b - t.a;
    float angle = ((atan(ab.y / ab.x)) * 180) / M_PI;
    t.rotate(-angle, vec2(0.0f, 0.0f));
    c.rotate(-angle, vec2(0.0f, 0.0f));
    rotation -= angle;

    // Ensure that c is above the line ab
    if (t.c.y - t.a.y < 0) {
        t.rotate(180.0f, vec2(0.0f, 0.0f));
        c.rotate(180.0f, vec2(0.0f, 0.0f));
        rotation += 180.0f;
    }

    // Swap a and b if b.x comes before a.x
    if (t.b.x < t.a.x) {
        vec2 a = t.a;
        vec2 b = t.b;
        t.a = b;
        t.b = a;
    }
    
    ab = t.b - t.a;
    ac = t.c - t.a;
    bc = t.c - t.b;

    std::cout << "A: (" << t.a.x << ", " << t.a.y << ")\n";
    std::cout << "B: (" << t.b.x << ", " << t.b.y << ")\n";
    std::cout << "C: (" << t.c.x << ", " << t.c.y << ")\n";

    return {false, vec2(0.0f, 0.0f), 0.0f};

}