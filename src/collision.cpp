#include <cmath>
#include <limits>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include "collision.hpp"

#include <iostream>

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

    bool below(vec2 point, vec2 start, vec2 end) {

        if (start.x == end.x) {return point.y < start.y;}

        float m = (end.y - start.y) / (end.x - start.x);
        float y = m * (point.x - start.x) + start.y;

        return point.y < y;

    }

    bool above(vec2 point, vec2 start, vec2 end) {

        if (start.x == end.x) {return point.y > start.y;}

        float m = (end.y - start.y) / (end.x - start.x);
        float y = m * (point.x - start.x) + start.y;

        return point.y > y;

    }

    bool intersects(vec2 point, Triangle localised) {
        return above(point, localised.a, localised.b) && below(point, localised.a, localised.c) && below(point, localised.c, localised.b);
    }

    bool intersects(vec2 aStart, vec2 aEnd, vec2 bStart, vec2 bEnd) {

        // Special Case: Both lines are vertical.
        if (aStart.x == aEnd.x && bStart.x == bEnd.x) {
            
            // If the x coordinates differ, they cannot intersect.
            if (aStart.x != bStart.x) {return false;}

            // If they're y ranges overlap, they intersect.
            float aMin = std::min(aStart.y, aEnd.y);
            float aMax = std::max(aStart.y, aEnd.y);
            float bMin = std::min(bStart.y, bEnd.y);
            float bMax = std::max(bStart.y, bEnd.y);

            return aMin < bMax && aMax > bMin;

        }

        // If B is vertical, swap A and B.
        if (bStart.x == bEnd.x) {
            
            vec2 tmp = aStart;
            aStart = bStart;
            bStart = tmp;

            tmp = aEnd;
            aEnd = bEnd;
            bEnd = tmp;

        }

        // Special Case: One Line is Vertical.
        if (aStart.x == bStart.x) {

            // Check if the vertical line is in B's horizontal range.
            float x = aStart.x;
            float bMin = std::min(bStart.x, bEnd.x);
            float bMax = std::max(bStart.x, bEnd.x);
            bool intersects = x >= bMin && x <= bMax;
            if (!intersects) {return false;}

            // Find the y value of B at A's x coordinate.
            float m = (bEnd.y - bStart.y) / (bEnd.x - bStart.x);
            float b = bStart.y - m * bStart.x;
            float y = m * x + b;

            // If the y value is not in A's vertical range, they do not intersect.
            float aMin = std::min(aStart.y, aEnd.y);
            float aMax = std::max(aStart.y, aEnd.y);
            intersects = y >= aMin && y <= aMax;
            if (!intersects) {return false;}

            return true;
        }

        // Standard Case: No vertical lines.
        
        // Line A -> ax + b
        float a = (aEnd.y - aStart.y) / (aEnd.x - aStart.x);
        float b = aStart.y - a * aStart.x;

        // Line B -> cx + d
        float c = (bEnd.y - bStart.y) / (bEnd.x - bStart.x);
        float d = bStart.y - c * bStart.x;

        // Intersection when ax + b = cx + d
        float x = (d - b) / (a - c);
        float y = a * x + b;

        // Check if the intersection occurs in both bounding boxes.
        vec2 min;
        vec2 max;
        bool intersects;

        // Line A
        min = vec2(std::min(aStart.x, aEnd.x), std::min(aStart.y, aEnd.y));
        max = vec2(std::max(aStart.x, aEnd.x), std::max(aStart.y, aEnd.y));
        intersects = x >= min.x && x <= max.x && y >= min.y && y <= max.y;
        if (!intersects) {return false;}

        // Line B
        min = vec2(std::min(bStart.x, bEnd.x), std::min(bStart.y, bEnd.y));
        max = vec2(std::max(bStart.x, bEnd.x), std::max(bStart.y, bEnd.y));
        intersects = x >= min.x && x <= max.x && y >= min.y && y <= max.y;
        if (!intersects) {return false;}

        return true;
    }

    vec2 normal(vec2 start, vec2 end) {
        vec2 result = end - start;
        rotateVector(result, 90.0f, vec2(0.0f, 0.0f));
        return glm::normalize(result);
    }

    float distance(vec2 point, vec2 start, vec2 end) {

        // Translate the space to the origin
        point -= start;
        end -= start;

        // Rotate the space such that start is on the x axis.
        float angle = ((atan(end.y / end.x)) * 180) / M_PI;
        rotateVector(point, -angle, vec2(0.0f, 0.0f));
        rotateVector(end, -angle, vec2(0.0f, 0.0f));

        // The perpendicular distance is the absolute value of point.y after rotation.
        return fabsf(point.y);

    }

    CollisionResult getCollision(Circle c, vec2 p) {

        vec2 difference = c.centre - p;
        if (glm::dot(difference, difference) < c.radius * c.radius) {
            
            vec2 normal = glm::normalize(difference);
            vec2 depthVector = ((normal * c.radius) - difference) * 0.5f;
            float depth = glm::length(depthVector);
            vec2 point = p - depthVector;

            return {true, normal, point, depth};
        }

        return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};
    }

    CollisionResult getCollision(Circle c, vec2 start, vec2 end) {

        // Keep track of any rotations and translations we make.
        float rotation = 0.0f;
        vec2 translation = vec2(0.0f, 0.0f);

        // Translate the space to the origin.
        c.centre -= start;
        end -= start;
        translation = -start;

        // Rotate the space such that end is on the x axis.
        float angle = ((atan(end.y / end.x)) * 180) / M_PI;
        rotateVector(c.centre, -angle, vec2(0.0f, 0.0f));
        rotateVector(end, -angle, vec2(0.0f, 0.0f));
        rotation = -angle;

        // If end.x is negative, let's rotate the plane by 180 degrees.
        if (end.x < 0) {
            rotateVector(c.centre, 180.0f, vec2(0.0f, 0.0f));
            rotateVector(end, 180.0f, vec2(0.0f, 0.0f));
            rotation += 180.0f;
        }

        // The perpendicular distance is the absolute value of p.y after rotation.
        float d = fabsf(c.centre.y);
        if (d >= c.radius) {return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};}

        // If the circle is not in the x range of the line, we do not consider it colliding.
        if (c.centre.x < 0 || c.centre.x > end.x) {return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};}
        if (c.centre.y < 0.0f || c.centre.y >= c.radius) {return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};}

        // Collision results.
        vec2 normal = vec2(0.0f, 1.0f);
        float depth = (c.radius - d) * 0.5f;
        vec2 point = vec2(c.centre.x, -depth);

        rotateVector(normal, -rotation ,vec2(0.0f, 0.0f));
        rotateVector(point, -rotation ,vec2(0.0f, 0.0f));
        point -= translation;

        return {true, normal, point, depth};
    }

}

CollisionResult getCollision(Circle a, Circle b) {

    // Determine if the two circles are colliding.
    float sumRadii = a.radius + b.radius;
    vec2 distance = a.centre - b.centre;
    if (glm::dot(distance, distance) - (sumRadii * sumRadii) > 0) {return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};}

    // Find the depth and normal of the collision
    float depth = fabsf(glm::length(distance) - sumRadii) * 0.5f;
    vec2 normal = glm::normalize(distance);

    // Find the contact point of the collision
    float distanceToPoint = a.radius - depth;
    vec2 point = distanceToPoint * -normal + a.centre;

    return {true, normal, point, depth};
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

    // Triangles in A's local space.
    TriangleLocalisation aLocalisation = localise(a);
    Triangle laa = aLocalisation.triangle;
    Triangle lab = Triangle(b.a, b.b, b.c);
    lab.rotate(aLocalisation.rotation, vec2(0.0f, 0.0f));
    lab.translate(aLocalisation.translation);

    // Triangles in B's local space.
    TriangleLocalisation bLocalisation = localise(b);
    Triangle lbb = bLocalisation.triangle;
    Triangle lba = Triangle(a.a, a.b, a.c);
    lba.rotate(bLocalisation.rotation, vec2(0.0f, 0.0f));
    lba.translate(bLocalisation.translation);

    // Find points in triangle B that collide with triangle A.
    vec2 aNormal = vec2(0.0f, 0.0f);
    vec2 aPoint = vec2(0.0f, 0.0f);
    int aPoints = 0;
    float aDepth = std::numeric_limits<float>::max();
    float i = 0.0f;

    if (intersects(lab.a, laa)) {

        if (intersects(b.a, b.b, a.a, a.b) || intersects(b.a, b.c, a.a, a.b)) {i = distance(b.a, a.a, a.b) * 0.5f; if (i < aDepth) {aNormal = normal(a.a, a.b); aDepth = i;}}
        if (intersects(b.a, b.b, a.a, a.c) || intersects(b.a, b.c, a.a, a.c)) {i = distance(b.a, a.a, a.c) * 0.5f; if (i < aDepth) {aNormal = normal(a.c, a.a); aDepth = i;}}
        if (intersects(b.a, b.b, a.b, a.c) || intersects(b.a, b.c, a.b, a.c)) {i = distance(b.a, a.b, a.c) * 0.5f; if (i < aDepth) {aNormal = normal(a.b, a.c); aDepth = i;}}

        aPoint += b.a;
        aPoints++;

    }

    if (intersects(lab.b, laa)) {

        if (intersects(b.b, b.c, a.a, a.b) || intersects(b.b, b.a, a.a, a.b)) {i = distance(b.b, a.a, a.b) * 0.5f; if (i < aDepth) {aNormal = normal(a.a, a.b); aDepth = i;}}
        if (intersects(b.b, b.c, a.a, a.c) || intersects(b.b, b.a, a.a, a.c)) {i = distance(b.b, a.a, a.c) * 0.5f; if (i < aDepth) {aNormal = normal(a.c, a.a); aDepth = i;}}
        if (intersects(b.b, b.c, a.b, a.c) || intersects(b.b, b.a, a.b, a.c)) {i = distance(b.b, a.b, a.c) * 0.5f; if (i < aDepth) {aNormal = normal(a.b, a.c); aDepth = i;}}
        
        aPoint += b.b;
        aPoints++;

    }

    if (intersects(lab.c, laa)) {

        if (intersects(b.c, b.a, a.a, a.b) || intersects(b.c, b.a, a.a, a.b)) {i = distance(b.c, a.a, a.b) * 0.5f; if (i < aDepth) {aNormal = normal(a.a, a.b); aDepth = i;}}
        if (intersects(b.c, b.a, a.a, a.c) || intersects(b.c, b.a, a.a, a.c)) {i = distance(b.c, a.a, a.c) * 0.5f; if (i < aDepth) {aNormal = normal(a.c, a.a); aDepth = i;}}
        if (intersects(b.c, b.a, a.b, a.c) || intersects(b.c, b.a, a.b, a.c)) {i = distance(b.c, a.b, a.c) * 0.5f; if (i < aDepth) {aNormal = normal(a.b, a.c); aDepth = i;}}

        aPoint += b.c;
        aPoints++;

    }

    if (aPoints == 3) {return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};}

    // Find points in triangle A that collide with triangle B.
    vec2 bNormal = vec2(0.0f, 0.0f);
    vec2 bPoint = vec2(0.0f, 0.0f);
    int bPoints = 0;
    float bDepth = std::numeric_limits<float>::max();
    float j = 0.0f;

    if (intersects(lba.a, lbb)) {

        if (intersects(a.a, a.b, b.a, b.b) || intersects(a.a, a.b, b.a, b.b)) {j = distance(a.a, b.a, b.b) * 0.5f; if (j < bDepth) {bNormal = normal(b.b, b.a); bDepth = j;}}
        if (intersects(a.a, a.b, b.a, b.c) || intersects(a.a, a.b, b.a, b.c)) {j = distance(a.a, b.a, b.c) * 0.5f; if (j < bDepth) {bNormal = normal(b.a, b.c); bDepth = j;}}
        if (intersects(a.a, a.b, b.b, b.c) || intersects(a.a, a.b, b.b, b.c)) {j = distance(a.a, b.b, b.c) * 0.5f; if (j < bDepth) {bNormal = normal(b.c, b.b); bDepth = j;}}

        bPoint += a.a;
        bPoints++;

    }

    if (intersects(lba.b, lbb)) {

        if (intersects(a.b, a.c, b.a, b.b) || intersects(a.b, a.a, b.a, b.b)) {j = distance(a.b, b.a, b.b) * 0.5f; if (j < bDepth) {bNormal = normal(b.b, b.a); bDepth = j;}}
        if (intersects(a.b, a.c, b.a, b.c) || intersects(a.b, a.a, b.a, b.c)) {j = distance(a.b, b.a, b.c) * 0.5f; if (j < bDepth) {bNormal = normal(b.a, b.c); bDepth = j;}}
        if (intersects(a.b, a.c, b.b, b.c) || intersects(a.b, a.a, b.b, b.c)) {j = distance(a.b, b.b, b.c) * 0.5f; if (j < bDepth) {bNormal = normal(b.c, b.b); bDepth = j;}}

        bPoint += a.b;
        bPoints++;

    }

    if (intersects(lba.c, lbb)) {
        
        if (intersects(a.c, a.a, b.a, b.b) || intersects(a.c, a.b, b.a, b.b)) {j = distance(a.c, b.a, b.b) * 0.5f; if (j < bDepth) {bNormal = normal(b.b, b.a); bDepth = j;}}
        if (intersects(a.c, a.a, b.a, b.c) || intersects(a.c, a.b, b.a, b.c)) {j = distance(a.c, b.a, b.c) * 0.5f; if (j < bDepth) {bNormal = normal(b.a, b.c); bDepth = j;}}
        if (intersects(a.c, a.a, b.b, b.c) || intersects(a.c, a.b, b.b, b.c)) {j = distance(a.c, b.b, b.c) * 0.5f; if (j < bDepth) {bNormal = normal(b.c, b.b); bDepth = j;}}

        bPoint += a.c;
        bPoints++;

    }

    if (bPoints == 3) {return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};}
    if (aPoints == 0 && bPoints == 0) {return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};}

    // If the more points in b were found than a, swap the problem
    if (aPoints < bPoints) {

        Triangle tmpTriangle = a;
        a = b;
        b = tmpTriangle;

        vec2 tmpNormal = aNormal;
        aNormal = bNormal;
        bNormal = tmpNormal;

        vec2 tmpPoint = aPoint;
        aPoint = bPoint;
        bPoint = tmpPoint;

        int tmpPoints = aPoints;
        aPoints = bPoints;
        bPoints = tmpPoints;

        float tmpDepth = aDepth;
        aDepth = bDepth;
        bDepth = tmpDepth;

    }

    if (aPoints == 1 && bPoints == 0) {
        vec2 pointToCentroid = b.getCentroid() - aPoint;
        pointToCentroid = glm::normalize(pointToCentroid);
        return {true, aNormal, aPoint + (aDepth * pointToCentroid), aDepth};
    }

    if (aPoints == 1 && bPoints == 1) {
        
        vec2 point = vec2((aPoint.x + bPoint.x) * 0.5f, (aPoint.y + bPoint.y) * 0.5f);
        vec2 normal = glm::normalize(aPoint - bPoint);
        float depth = glm::length(bPoint - aPoint) * 0.5f;

        if (aDepth < depth) {
            normal = aNormal;
            depth = aDepth;
        }

        if (bDepth < depth) {
            normal = bNormal;
            depth = bDepth;
        }

        return {true, normal, point, depth};
    }

    if (aPoints == 2 && bPoints == 0) {
        vec2 point = vec2(aPoint.x * 0.5f, aPoint.y * 0.5f) - aDepth * aNormal;
        return {true, aNormal, point, aDepth};
    }

    if (aPoints == 2 && bPoints == 1) {
        aPoint = vec2(aPoint.x * 0.5f, aPoint.y * 0.5f);
        vec2 normal = glm::normalize(aPoint - bPoint);
        vec2 point = vec2((aPoint.x + bPoint.x) * 0.5f, (aPoint.y + bPoint.y) * 0.5f);
        float depth = glm::length(bPoint - aPoint) * 0.5f;
        return {true, normal, point, depth};
    }

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
    
    CollisionResult result; 

    // Check if the circle collides with any edges.
    result = getCollision(c, t.a, t.c); if (result.colliding) {return result;}
    result = getCollision(c, t.c, t.b); if (result.colliding) {return result;}
    result = getCollision(c, t.b, t.a); if (result.colliding) {return result;}
    
    // Check if the circle collides with any corners.
    result = getCollision(c, t.a); if (result.colliding) {return result;}
    result = getCollision(c, t.b); if (result.colliding) {return result;}
    result = getCollision(c, t.c); if (result.colliding) {return result;}
    
    return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};
}

CollisionResult getCollision(Triangle t, Circle c) {
    CollisionResult result = getCollision(c, t);
    result.normal = -result.normal;
    return result;
}