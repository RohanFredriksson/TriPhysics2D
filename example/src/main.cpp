#include "window.hpp"

#include <iostream>
#include "trip2d/trip2d.hpp"

int main() {

    Triangle t = Triangle(vec2(0.0f, 1.0f), vec2(1.0f, 0.0f), vec2(2.0f, 2.0f));
    Circle c = Circle(1.0f, vec2(0.0f, 0.0f));

    getCollision(c, t);

    if(!Window::init()) {return 1;}
    Window::loop();
    Window::destroy();
    return 0;

}