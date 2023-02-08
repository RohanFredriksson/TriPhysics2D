#pragma once

#include "camera.hpp"

namespace Window {
    
    bool init();
    void loop();
    void destroy();

    int getWidth();
    int getHeight();
    float getAspectRatio();
    Camera* getCamera();

    void setWidth(int w);
    void setHeight(int h);

}