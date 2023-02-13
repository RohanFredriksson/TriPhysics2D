#include "window.hpp"

#include <iostream>
#include "trip2d/trip2d.hpp"

int main() {

    Triangle t = Triangle(vec2(2.5f, 1.0f), vec2(5.5f, 1.5f), vec2(3.5f, 2.5f));
    Circle c = Circle(1.0f, vec2(2.4f, 2.3f));

    CollisionResult result = getCollision(c, t);
    std::cout << "Colliding: " << result.colliding << "\n";
    std::cout << "Normal: (" << result.normal.x << ", " << result.normal.y << ")\n";
    std::cout << "Contact Point: (" << result.point.x << ", " << result.point.y << ")\n";
    std::cout << "Depth: " << result.depth << "\n";

    if(!Window::init()) {return 1;}
    Window::loop();
    Window::destroy();
    return 0;

}