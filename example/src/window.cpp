#include <iostream>
#include <cstdlib>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "trip2d/trip2d.hpp"

#include "window.hpp"
#include "listener.hpp"
#include "shader.hpp"
#include "renderer.hpp"

namespace {

    GLFWwindow* window;
    Camera* camera;
    int width = 800;
    int height = 800;

    Circle circle = Circle(1.0f, vec2(0.0f, 0.0f));
    Triangle triangle = Triangle(vec2(-1.5f, -1.0f), vec2(2.5f, -0.5f), vec2(-0.5f, 0.5f));
    CollisionResult result;

    void update(float dt) {

        if (MouseListener::isMouseDragging()) {
            if (MouseListener::getDx() != 0) {camera->addPosition(vec2(-MouseListener::getWorldDx(), 0.0f));}
            if (MouseListener::getDy() != 0) {camera->addPosition(vec2(0.0f, -MouseListener::getWorldDy()));}
        }

        if (MouseListener::getScrollY() != 0.0) {
            triangle.rotate(5.0f * MouseListener::getScrollY(), vec2(0.0f, 0.0f));
        }

        circle.centre.x = (float) MouseListener::getWorldX();
        circle.centre.y = (float) MouseListener::getWorldY();

        result = getCollision(circle, triangle);

    }

    void render() {

        // Render the scene to the window.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (result.colliding) {
            Renderer::drawCircle(circle.centre, circle.radius, vec3(0.0f, 1.0f, 0.0f), 1);
            Renderer::drawTriangle(triangle.a, triangle.b, triangle.c, vec3(0.0f, 1.0f, 0.0f), 1);
            Renderer::drawLine(result.point, result.point + result.normal, vec3(0.0f, 1.0f, 0.0f), 1);
        } 

        else {
            Renderer::drawCircle(circle.centre, circle.radius, vec3(1.0f, 0.0f, 0.0f), 1);
            Renderer::drawTriangle(triangle.a, triangle.b, triangle.c, vec3(1.0f, 0.0f, 0.0f), 1);
        }

        // Render the lines.
        Renderer::render();

        glfwSwapBuffers(window);

    }

}

namespace Window {

    bool init() {

        // Initialise GLFW
        if (!glfwInit()) {
            std::cout << "ERROR::WINDOW::GLFW_INITIALIZATION_FAILED\n";
            return false;
        }

        // Configure GLFW
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);

        // Create the window
        window = glfwCreateWindow(width, height, "Pancake", NULL, NULL);
        if (window == NULL) {
            std::cout << "ERROR::WINDOW::GLFW_WINDOW_CREATION_FAILED\n";
            return false;
        }

        // Manage callbacks
        glfwSetKeyCallback(window, KeyListener::keyCallback);
        glfwSetWindowSizeCallback(window, WindowListener::resizeCallback);
        glfwSetCursorPosCallback(window, MouseListener::mousePosCallback);
        glfwSetMouseButtonCallback(window, MouseListener::mouseButtonCallback);
        glfwSetScrollCallback(window, MouseListener::mouseScrollCallback);

        // Make the OpenGl context current
        glfwMakeContextCurrent(window);

        // Enable v-sync
        glfwSwapInterval(1);

        // Make the window visible
        glfwShowWindow(window);

        // Load GLAD so it configures OpenGL
        gladLoadGL();

        // Enable alpha transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        // Initialise the camera
        camera = new Camera(vec2(0.0f, 0.0f), vec2(10.0f, 10.0f), 1.0f);

        // Initialise the renderer
        Renderer::init();

        return true;
    }

    void loop() {

        float beginTime = (float)glfwGetTime();
        float endTime = (float)glfwGetTime();
        float dt = -1.0f;

        while (!glfwWindowShouldClose(window)) {

            glfwPollEvents();

            if (dt > 0) {
                update(dt);
                render();
            }

            MouseListener::endFrame();

            endTime = (float)glfwGetTime();
            dt = endTime - beginTime;
            beginTime = endTime;

        }

    }

    void destroy() {
        Renderer::destroy();
        delete camera;
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    float getAspectRatio() {
        return (float) width / (float) height;
    }

    Camera* getCamera() {
        return camera;
    }

    void setWidth(int w) {
        width = w;
    }

    void setHeight(int h) {
        height = h;
    }

}