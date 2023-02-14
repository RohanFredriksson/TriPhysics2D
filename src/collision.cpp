#include <cmath>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include "collision.hpp"

namespace {

    struct TriangleLocalisation {
        Triangle triangle;
        float rotation;
        vec2 translation;
    };

    TriangleLocalisation localise(Triangle t) {

        // Keep track of the translation and rotation.
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
            rotation += 90.0f;
        }

        // Rotate the space such that ab is horizontal
        ab = t.b - t.a;
        float angle = ((atan(ab.y / ab.x)) * 180) / M_PI;
        t.rotate(-angle, vec2(0.0f, 0.0f));
        rotation -= angle;

        // Ensure that c is above the line ab
        if (t.c.y - t.a.y < 0) {
            t.rotate(180.0f, vec2(0.0f, 0.0f));
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
        t.translate(translation);

        // Return the triangle.
        TriangleLocalisation result = {t, rotation, translation};
        return result;

    }

}

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

CollisionResult getCollision(Triangle a, Triangle b) {

    // Do a bounding box check to try see if a collision is possible
    bool colliding = false;
    vec2 aMin = vec2(std::min(std::min(a.a.x, a.b.x), a.c.x), std::min(std::min(a.a.y, a.b.y), a.c.y));
    vec2 aMax = vec2(std::max(std::max(a.a.x, a.b.x), a.c.x), std::max(std::max(a.a.y, a.b.y), a.c.y));
    vec2 bMin = vec2(std::min(std::min(b.a.x, b.b.x), b.c.x), std::min(std::min(b.a.y, b.b.y), b.c.y));
    vec2 bMax = vec2(std::max(std::max(b.a.x, b.b.x), b.c.x), std::max(std::max(b.a.y, b.b.y), b.c.y));

    // Check if both triangle aabbs overlap.
    colliding = aMin.x < bMax.x && aMax.x > bMin.x && aMin.y < bMax.y && aMax.y > bMin.y;
    if (!colliding) {return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};}



    return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};
}

CollisionResult getCollision(Circle c, Triangle t) {

    // Do a bounding box check to try see if a collision is possible
    bool colliding = false;
    vec2 min = vec2(std::min(std::min(t.a.x, t.b.x), t.c.x) - c.radius, std::min(std::min(t.a.y, t.b.y), t.c.y) - c.radius);
    vec2 max = vec2(std::max(std::max(t.a.x, t.b.x), t.c.x) + c.radius, std::max(std::max(t.a.y, t.b.y), t.c.y) + c.radius);

    // Check if the circle is in the triangles aabb
    colliding = c.centre.x >= min.x && c.centre.x <= max.x && c.centre.y >= min.y && c.centre.y <= max.y;
    if (!colliding) {return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};}

    // Create a copy of the triangle in local space.
    TriangleLocalisation localisation = localise(t);
    Triangle lt = localisation.triangle;
    float rotation = localisation.rotation;
    vec2 translation = localisation.translation;
    
    // Move the circle into local space.
    Circle lc = c;
    lc.rotate(rotation, vec2(0.0f, 0.0f));
    lc.translate(translation);

    // Check if the circle is colliding with ab.
    colliding = lc.centre.x >= lt.a.x && lc.centre.x <= lt.b.x && lc.centre.y <= 0.0f && lc.centre.y > -lc.radius;
    if (colliding) {
        
        // Get the depth, normal, contact point information.
        float depth = (lc.radius + lc.centre.y) * 0.5f;
        vec2 normal = vec2(0.0f, -1.0f); // MAYBE FLIP THIS?
        vec2 point = vec2(lc.centre.x, depth);

        // Translate back into global space.
        point -= translation;
        rotateVector(normal, -rotation, vec2(0.0f, 0.0f));
        rotateVector(point, -rotation, vec2(0.0f, 0.0f));

        return {true, normal, point, depth};
    }

    // Check if the circle is colliding with ac.
    vec2 lac = lt.c - lt.a;
    vec2 centre = lc.centre;
    float angle = ((atan(lac.y / lac.x)) * 180) / M_PI;
    rotateVector(centre, -angle, vec2(0.0f, 0.0f));

    colliding = centre.x >= 0.0f && centre.x <= glm::length(lt.c) && centre.y >= 0.0f && centre.y < lc.radius;
    if (colliding) {

        // Get the depth, normal, contact point information.
        float depth = (lc.radius - centre.y) * 0.5f;
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
    vec2 lbc = lt.c - lt.b;
    centre = lc.centre;
    angle = ((atan(lbc.y / lbc.x)) * 180) / M_PI;
    rotateVector(centre, -angle, lt.b);

    colliding = centre.x >= lt.b.x - glm::length(lbc) && centre.x <= lt.b.x && centre.y >= 0.0f && centre.y < lc.radius;
    if (colliding) {

        // Get the depth, normal, contact point information.
        float depth = (lc.radius - centre.y) * 0.5f;
        vec2 normal = vec2(0.0f, 1.0f); // MAYBE FLIP THIS?
        vec2 point = vec2(centre.x, -depth);

        // Rotate the normal and point by the angle.
        rotateVector(normal, angle, vec2(0.0f, 0.0f));
        rotateVector(point, angle, lt.b);

        // Translate back into global space.
        point -= translation;
        rotateVector(normal, -rotation, vec2(0.0f, 0.0f));
        rotateVector(point, -rotation, vec2(0.0f, 0.0f));

        return {true, normal, point, depth};
    }

    // Check if the circle collides with any corners.
    CollisionResult result; 

    // Check if the circle is colliding with a.
    result = getCollision(c, t.a);
    if (result.colliding) {return result;}

    // Check if the circle is colliding with b.
    result = getCollision(c, t.b);
    if (result.colliding) {return result;}

    // Check if the circle is colliding with c.
    result = getCollision(c, t.c);
    if (result.colliding) {return result;}

    return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};
}

CollisionResult getCollision(Triangle t, Circle c) {
    CollisionResult result = getCollision(c, t);
    result.normal = -result.normal;
    return result;
}

CollisionResult getCollision(Circle c, vec2 p) {

    vec2 difference = c.centre - p;
    if (glm::dot(difference, difference) < c.radius * c.radius) {
        
        vec2 normal = glm::normalize(difference); // MAYBE FLIP THIS?
        vec2 depthVector = ((normal * c.radius) - difference) * 0.5f;
        float depth = glm::length(depthVector);
        vec2 point = p - depthVector;

        return {true, normal, point, depth};
    }

    return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};
}