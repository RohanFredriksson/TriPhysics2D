#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include "collision.hpp"

#include <iostream>

namespace {

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

    vec2 getNormal(vec2 start, vec2 end) {
        vec2 result = end - start;
        rotateVector(result, 90.0f, vec2(0.0f, 0.0f));
        return glm::normalize(result);
    }

    /*
    Gets distance from aStart to the intersection of the lines A and B.
    This function assumes that we are working with infinitely long lines, and NOT line segments.
    To determine whether two line segments intersect, use: 

        intersects(vec2 aStart, vec2 aEnd, vec2 bStart, vec2 bEnd);
    */
    float getDistance(vec2 aStart, vec2 aEnd, vec2 bStart, vec2 bEnd) {

        // If either line is vertical, rotate the problem 45 degrees.
        // Since we are solving for distance, direction does not need to be preserved.
        if (aStart.x == aEnd.x || bStart.x == bEnd.x) {
            rotateVector(aStart, 45.0f, vec2(0.0f, 0.0f));
            rotateVector(aEnd, 45.0f, vec2(0.0f, 0.0f));
            rotateVector(bStart, 45.0f, vec2(0.0f, 0.0f));
            rotateVector(bEnd, 45.0f, vec2(0.0f, 0.0f));
        }

        // y = ax + b
        float a = (aEnd.y - aStart.y) / (aEnd.x - aStart.x);
        float b = aStart.y - a * aStart.x;
        
        // y = cx + d
        float c = (bEnd.y - bStart.y) / (bEnd.x - bStart.x);
        float d = bStart.y - c * bStart.x;

        // ax + b = cx + d: solve for x
        float x = (d - b) / (a - c);

        // plug x into ax + b
        float y = a * x + b;

        // Now that we have the point, compute the distance between points.
        vec2 point = vec2(x, y);
        return glm::distance(aStart, point);

    }

    float getPerpendicularDistance(vec2 point, vec2 start, vec2 end) {

        // If the line is vertical, rotate the problem 45 degrees.
        // Since we are solving for distance, direction does not need to be preserved.
        if (start.x == end.x) {
            rotateVector(point, 45.0f, vec2(0.0f, 0.0f));
            rotateVector(start, 45.0f, vec2(0.0f, 0.0f));
            rotateVector(end, 45.0f, vec2(0.0f, 0.0f));
        }

        // Get the line in form y = mx + b
        float gradient = (end.y - start.y) / (end.x - start.x);
        float intercept = start.y - gradient * start.x;

        // Get the line in general form ax + by + c = 0
        float a = 1.0f;
        float b = -gradient;
        float c = -intercept;

        // Use perpendicular distance formula.
        float distance = fabsf(a * point.x + b * point.y + c) / sqrtf(a * a + b * b);
        return distance;

    }

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

    struct IntersectionResult {
        bool intersects;
        vec2 point;
    };

    IntersectionResult getIntersection(vec2 aStart, vec2 aEnd, vec2 bStart, vec2 bEnd) {

        aEnd -= aStart;
        bStart -= aStart;
        bEnd -= aStart;

        float angle = -(atan(aEnd.y / aEnd.x) * 180) / M_PI;
        rotateVector(aEnd, angle, vec2(0.0f, 0.0f));

        if (aEnd.x < 0) {
            rotateVector(aEnd, 180.0f, vec2(0.0f, 0.0f));
            angle += 180.0f;
        }

        rotateVector(bStart, angle, vec2(0.0f, 0.0f));
        rotateVector(bEnd, angle, vec2(0.0f, 0.0f));

        float m = (bEnd.y - bStart.y) / (bEnd.x - bStart.x);
        float b = bStart.y - m * bStart.x;
        float x = -b / m;

        if (x >= 0.0f && x <= aEnd.x) {

            vec2 point = vec2(x, 0.0f);
            rotateVector(point, -angle, vec2(0.0f, 0.0f));
            point += aStart;

            return {true, point};
        }

        return {false, vec2(0.0f, 0.0f)};
    }

    bool intersects(vec2 aStart, vec2 aEnd, vec2 bStart, vec2 bEnd) {
        float dx0 = aEnd.x - aStart.x;
        float dx1 = bEnd.x - bStart.x;
        float dy0 = aEnd.y - aStart.y;
        float dy1 = bEnd.y - bStart.y;
        float p0 = dy1 * (bEnd.x - aStart.x) - dx1 *(bEnd.y - aStart.y);
        float p1 = dy1 * (bEnd.x - aEnd.x)   - dx1 *(bEnd.y - aEnd.y);
        float p2 = dy0 * (aEnd.x - bStart.x) - dx0 *(aEnd.y - bStart.y);
        float p3 = dy0 * (aEnd.x - bEnd.x)   - dx0 *(aEnd.y - bEnd.y);
        return (p0 * p1 <= 0) && (p2 * p3 <= 0);
    }

    std::vector<Line> getIntersectingEdge(vec2 vertex, Triangle b, Triangle a) {

        std::vector<Line> result;

        Line edge = b.left(vertex);
        if (intersects(edge.start, edge.end, a.b, a.a)) {result.push_back(Line(a.b, a.a));}
        if (intersects(edge.start, edge.end, a.a, a.c)) {result.push_back(Line(a.a, a.c));}
        if (intersects(edge.start, edge.end, a.c, a.b)) {result.push_back(Line(a.c, a.b));}

        edge = b.right(vertex);
        if (intersects(edge.start, edge.end, a.b, a.a)) {result.push_back(Line(a.b, a.a));}
        if (intersects(edge.start, edge.end, a.a, a.c)) {result.push_back(Line(a.a, a.c));}
        if (intersects(edge.start, edge.end, a.c, a.b)) {result.push_back(Line(a.c, a.b));}

        return result;

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
    std::vector<vec2> aPoints;
    if (intersects(lab.a, laa)) {aPoints.push_back(b.a);}
    if (intersects(lab.b, laa)) {aPoints.push_back(b.b);}
    if (intersects(lab.c, laa)) {aPoints.push_back(b.c);}

    if (aPoints.size() == 3) {return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};}

    // Find points in triangle A that collide with triangle B.
    std::vector<vec2> bPoints;
    if (intersects(lba.a, lbb)) {bPoints.push_back(a.a);}
    if (intersects(lba.b, lbb)) {bPoints.push_back(a.b);}
    if (intersects(lba.c, lbb)) {bPoints.push_back(a.c);}

    if (bPoints.size() == 3) {return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};}
    if (aPoints.size() == 0 && bPoints.size() == 0) {return {false, vec2(0.0f, 0.0f), vec2(0.0f, 0.0f), 0.0f};}

    // If the more points in b were found than a, swap the problem
    float flip = -1.0f;
    if (aPoints.size() < bPoints.size()) {

        // Swap the triangles
        Triangle t = a;
        a = b;
        b = t;

        // Swap the vectors
        std::vector<vec2> points;
        aPoints = bPoints;
        bPoints = points;

        // Flip the float
        flip = 1.0f;

    }

    if (aPoints.size() == 1 && bPoints.size() == 0) {

        // TODO IF THERE ARE MORE THAN 1 EDGES USE THE SMALLEST ONE.

        vec2 vertex = aPoints[0];
        Line edge = getIntersectingEdge(vertex, b, a)[0];
        vec2 centroid = b.centroid();
        
        vec2 normal = flip * getNormal(edge.start, edge.end);
        float depth = getDistance(vertex, centroid, edge.start, edge.end) * 0.5f;
        vec2 point = vertex + glm::normalize(centroid - vertex) * depth;
    
        return {true, normal, point, depth};
    }

    if (aPoints.size() == 1 && bPoints.size() == 1) {

        // Find the closest edge in triangle A to the point of B that intersects A.
        float bestDistance = std::numeric_limits<float>::max();
        float distance = 0.0f;
        Line bestEdge = Line(vec2(0.0f, 0.0f), vec2(0.0f, 0.0f));
        Triangle bestTriangle = b;
        
        // The point of collision is the average of both vertices that collided.
        vec2 vertex = (aPoints[0] + bPoints[0]) * 0.5f;
        
        // Find the closest edge in A to the point of collision.
        std::vector<Line> aEdges = getIntersectingEdge(aPoints[0], b, a);
        distance = getDistance(aPoints[0], a.left(aPoints[0]).end, aEdges[0].start, aEdges[0].end);
        if (distance < bestDistance) {
            bestDistance = distance;
            bestEdge = aEdges[0];
            bestTriangle = b;
            flip = 1.0f;
        }

        distance = getDistance(aPoints[0], a.right(aPoints[0]).end, aEdges[1].start, aEdges[1].end);
        if (distance < bestDistance) {
            bestDistance = distance;
            bestEdge = aEdges[1];
            bestTriangle = a;
            flip = 1.0f;
        }
        
        // See if any edge in B is closer.
        std::vector<Line> bEdges = getIntersectingEdge(bPoints[0], a, b);
        distance = getDistance(bPoints[0], b.left(bPoints[0]).end, bEdges[0].start, bEdges[0].end);
        if (distance < bestDistance) {
            bestDistance = distance;
            bestEdge = bEdges[0];
            bestTriangle = a;
            flip = -1.0f;
        }
        
        distance = getDistance(bPoints[0], b.right(bPoints[0]).end, bEdges[1].start, bEdges[1].end);
        if (distance < bestDistance) {
            bestDistance = distance;
            bestEdge = bEdges[1];
            bestTriangle = a;
            flip = -1.0f;
        }

        vec2 normal = flip * getNormal(bestEdge.end, bestEdge.start);
        vec2 centroid = bestTriangle.centroid();
        float depth = getDistance(vertex, centroid, bestEdge.start, bestEdge.end) * 0.5f;

        return {true, normal, vertex, depth};
    }

    if (aPoints.size() == 2) {

        vec2 vertex = 0.5f * (aPoints[0] + aPoints[1]);
        Line edge = getIntersectingEdge(aPoints[0], b, a)[0];
        vec2 centroid = b.centroid();

        vec2 normal = flip * getNormal(aPoints[0], aPoints[1]);
        float depth = getDistance(vertex, centroid, edge.start, edge.end) * 0.5f;
        vec2 point = vertex + glm::normalize(centroid - vertex) * depth;

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