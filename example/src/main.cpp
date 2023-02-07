#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

GLFWwindow* window;
int width = 800;
int height = 800;

void update(float dt) {
    
}

void render() {

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    //Renderer::bindShader(defaultShader);
    //scene->render();

    glfwSwapBuffers(window);

}

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

    /*
    // Manage callbacks
    glfwSetKeyCallback(window, KeyListener::keyCallback);
    glfwSetWindowSizeCallback(window, WindowListener::resizeCallback);
    glfwSetCursorPosCallback(window, MouseListener::mousePosCallback);
    glfwSetMouseButtonCallback(window, MouseListener::mouseButtonCallback);
    glfwSetScrollCallback(window, MouseListener::mouseScrollCallback);
    */

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

    /*
    defaultShader = new Shader("assets/shaders/default.vert", "assets/shaders/default.frag");
    */
    
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

        endTime = (float)glfwGetTime();
        dt = endTime - beginTime;
        beginTime = endTime;

    }

}

void destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main() {
    if (!init()) {return 1;}
    loop();
    destroy();
    return 0;
}