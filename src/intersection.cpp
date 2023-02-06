#include <algorithm>
#include "intersection.hpp"

IntersectionResult isIntersecting(Line l1, Line l2) {

    // Special Case: Both lines are vertical.
    if (l1.isVertical() && l2.isVertical()) {

        // If the x coordinates of each line differ, they cannot intersect.
        if (l1.getFrom().x != l2.getFrom().x) {return { false, vec2(0.0f, 0.0f) };}

        // Ensure that l1 is the "lower" box of the two.
        if (l1.getMin().y > l2.getMin().y) {
            Line tmp = l1;
            l1 = l2;
            l2 = tmp;
        }

        // If the bounding boxes dont intersect, they cannot intersect.
        float l1Start = l1.getMin().y;
        float l1End = l1.getMax().y;
        float l2Start = l2.getMin().y;
        float l2End = l2.getMax().y;
        bool intersects = l1End >= l2Start && l1Start <= l2End;
        if (!intersects) {return { false, vec2(0.0f, 0.0f) };}
        
        // If reached here, then the lines intersect.
        return { true, vec2(l1.getFrom().x, (l1End + l2Start) / 2.0f) };

    }

    else {

        // If Line 2 is a vertical line, then swap Line 1 and Line 2.
        if (l2.isVertical()) {
            Line tmp = l1;
            l1 = l2;
            l2 = tmp;
        }

        // Special Case: One line is vertical.
        if (l1.isVertical()) {

            // If Line 2's bounding box does not include Line 1's x coordinate, they do not intersect.
            float x = l1.getFrom().x;
            bool intersects = x >= l2.getMin().x && x <= l2.getMax().x;
            if (!intersects) {return { false, vec2(0.0f, 0.0f) };}

            // Find the y value of Line 2 at Line 1's x coordinate
            float m = l2.getGradient();
            float b = l2.getIntercept();
            float y = m * x + b;

            // If the y value is not in Line 1's vertical range, they do not intersect.
            intersects = y >= l2.getMin().y && y <= l2.getMax().y;
            if (!intersects) {return { false, vec2(0.0f, 0.0f) };}

            // If reached here, then the lines intersect.
            return { true, vec2(x, y) };

        }

        // Standard Case: No vertical lines.
        else {

            // Line 1 -> ax + b
            float a = l1.getGradient();
            float b = l1.getIntercept();

            // Line 2 -> cx + d
            float c = l2.getGradient();
            float d = l2.getIntercept();

            // Intersection when ax + b = cx + d
            float x = (d - b) / (a - c);
            float y = a * x + b;

            // Check if the intersection occurs in both bounding boxes.
            vec2 min;
            vec2 max;
            bool intersects;

            // Line 1
            min = l1.getMin();
            max = l1.getMax();
            intersects = x >= min.x && x <= max.x && y >= min.y && y <= max.y;
            if (!intersects) {return { false, vec2(0.0f, 0.0f) };}

            // Line 2
            min = l2.getMin();
            max = l2.getMax();
            intersects = x >= min.x && x <= max.x && y >= min.y && y <= max.y;
            if (!intersects) {return { false, vec2(0.0f, 0.0f) };}

            return { true, vec2(x, y) };

        }
        
    }

}