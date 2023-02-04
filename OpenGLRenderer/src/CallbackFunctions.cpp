// GLFW callback functions - Julian Haurin - 2/4/23

#include <CallbackFunctions.h>

// recalculates the viewport dimensions each time the OpenGl GLFW window is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {

    // glViewPort is used to transform the 2D coordinates OpenGL processes to points on your screen (behind the scene)
    glViewport(0, 0, width, height);

}

// handles GLFW errors using glfwSetErrorCallback
void glfwErrorCallack(int code, const char* description) {

    std::cout << "GLFW error reported: " << description << std::endl;

}
