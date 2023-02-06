// General functions - Julian Haurin - 2/6/23 //

#include "Functions.h"

// Sets up opengl - intializes GLFW window and OpenGl context, loads glew function pointers //
GLFWwindow* setupOpenGL(const unsigned int in_screenWidth, const unsigned int in_screenHeight) {

    // Initializes and configures the GLFW library and OpenGL context //

    if (glfwInit() == GLFW_FALSE) {
        std::cout << "[J] ERROR: GLFW failed to initialize " << std::endl;
        return NULL;
    }
    std::cout << "[J] GLFW successfully initialized! " << std::endl;

    // checks GLFW version
    std::cout << "[J] GLFW compile-time version: " << GLFW_VERSION_MAJOR << GLFW_VERSION_MINOR << GLFW_VERSION_REVISION << std::endl;
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    std::cout << "[J] GLFW run-time version: " << major << minor << revision << std::endl;

    // GLFW window specifications 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // initializes opengl version as 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // program is run in core-profile 

    // Creates a window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(in_screenWidth, in_screenHeight, "OpenGL 3D Renderer", NULL, NULL);
    if (window == NULL) {
        std::cout << "[J] ERROR: failed to create valid OpenGL window using GLFW " << std::endl;
        glfwTerminate();
        return NULL;
    }
    std::cout << "[J] GLFW window created! " << std::endl;

    // Makes the window context current
    glfwMakeContextCurrent(window);
    std::cout << "[J] OpenGL context created and initialized! " << std::endl;


    // Initialize GLEW (after valid OpenGL context has been created) to load all OpenGl function pointers // --------------------
    if (glewInit() != GLEW_OK) {
        std::cout << "[J] ERROR: glewInit() did not return GLEW_OK " << std::endl;
    }

    std::cout << "[J] GLEW initialized! " << std::endl;
    std::cout << "[J] OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    return window; // on success
}

// Callback Functions // --------------------------------------------------------------------------------------------------------

// recalculates the viewport dimensions each time the OpenGl GLFW window is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {

    // glViewPort is used to transform the 2D coordinates OpenGL processes to points on your screen (behind the scene)
    glViewport(0, 0, width, height);

}

// handles GLFW errors using glfwSetErrorCallback
void glfwErrorCallack(int code, const char* description) {

    std::cout << "[J] ERROR: GLFW error reported: " << description << std::endl;

}