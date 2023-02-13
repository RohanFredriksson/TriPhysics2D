#include <cmath>
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include "collision.hpp"

#include <iostream>

CollisionResult getCollision(Circle a, Circle b) {

    // Determine if the two circles are colliding.
    float sumRadii = a.radius + b.radius;
    vec2 distance = b.centre - a.centre;
    if (glm::dot(distance, distance) - (sumRadii * sumRadii) > 0) {return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};}

    // Find the depth and normal of the collision
    float depth = fabsf(glm::length(distance) - sumRadii) * 0.5f;
    vec2 normal = glm::normalize(distance);

    // Find the contact point of the collision
    float distanceToPoint = a.radius - depth;
    vec2 contactPoint = distanceToPoint * normal + a.centre;

    return {true, normal, contactPoint, depth};

}

CollisionResult getCollision(Circle c, Triangle t) {

    // Keep track of the translation that occurs.
    bool colliding = false;
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

    // Translate the space such that a is at the origin.
    translation = -t.a;
    c.translate(translation);
    t.translate(translation);

    // Check if the circle is colliding with ab.
    colliding = c.centre.x >= t.a.x && c.centre.x <= t.b.x && c.centre.y <= 0.0f && c.centre.y > -c.radius;
    if (colliding) {
        
        // Get the depth, normal, contact point information.
        float depth = (c.radius + c.centre.y) * 0.5f;
        vec2 normal = vec2(0.0f, -1.0f); // MAYBE FLIP THIS?
        vec2 point = vec2(c.centre.x, depth);

        // Translate back into global space.
        point -= translation;
        rotateVector(normal, -rotation, vec2(0.0f, 0.0f));
        rotateVector(point, -rotation, vec2(0.0f, 0.0f));

        return {true, normal, point, depth};
    }

    // Check if the circle is colliding with ac.
    ac = t.c - t.a;
    vec2 centre = c.centre;
    angle = ((atan(ac.y / ac.x)) * 180) / M_PI;
    rotateVector(centre, -angle, vec2(0.0f, 0.0f));

    colliding = centre.x >= 0.0f && centre.x <= glm::length(t.c) && centre.y >= 0.0f && centre.y < c.radius;
    if (colliding) {

        // Get the depth, normal, contact point information.
        float depth = (c.radius - centre.y) * 0.5f;
        vec2 normal = vec2(0.0f, 1.0f); // MAYBE FLIP THIS?
        vec2 point = vec2(centre.x, -depth);

        // Rotate the normal and point by the angle.
        rotateVector(normal, angle, vec2(0.0f, 0.0f));
        rotateVector(point, angle, vec2(0.0f, 0.0f));

        // Translate back into global space.
        point -= translation;
        rotateVector(normal, -rotation, vec2(0.0f, 0.0f));
        rotateVector(point, -rotation, vec2(0.0f, 0.0f));

        return {true, normal, point, depth};
    }

    // Check if the circle is colliding with bc.
    bc = t.c - t.b;
    centre = c.centre;
    angle = ((atan(bc.y / bc.x)) * 180) / M_PI;
    rotateVector(centre, -angle, t.b);

    colliding = centre.x >= t.b.x - glm::length(bc) && centre.x <= t.b.x && centre.y >= 0.0f && centre.y < c.radius;
    if (colliding) {

        // Get the depth, normal, contact point information.
        float depth = (c.radius - centre.y) * 0.5f;
        vec2 normal = vec2(0.0f, 1.0f); // MAYBE FLIP THIS?
        vec2 point = vec2(centre.x, -depth);

        // Rotate the normal and point by the angle.
        rotateVector(normal, angle, t.b);
        rotateVector(point, angle, t.b);

        // Translate back into global space.
        point -= translation;
        rotateVector(normal, -rotation, vec2(0.0f, 0.0f));
        rotateVector(point, -rotation, vec2(0.0f, 0.0f));

        return {true, normal, point, depth};
    }

    return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};

}