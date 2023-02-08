#include "window.hpp"

int main() {
    if(!Window::init()) {return 1;}
    Window::loop();
    Window::destroy();
    return 0;
}