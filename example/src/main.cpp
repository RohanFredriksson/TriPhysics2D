#include "window.hpp"

#include <iostream>
#include "trip2d/trip2d.hpp"

int main() {

    Circle a = Circle(1.0f, vec2(0.0f, 0.0f));
    std::cout << a.radius << "\n";

    if(!Window::init()) {return 1;}
    Window::loop();
    Window::destroy();
    return 0;

}