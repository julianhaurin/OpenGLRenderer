// General functions - Julian Haurin - 2/6/23 //

#include "Functions.h"

// Sets up opengl - intializes GLFW window and OpenGl context, loads glew function pointers //
GLFWwindow* setupOpenGL(const unsigned int in_screenWidth, const unsigned int in_screenHeight) {

    // Initializes and configures the GLFW library and OpenGL context //

    if (glfwInit() == GLFW_FALSE) {
        std::cout << "ERROR: GLFW failed to initialize" << std::endl;
        return NULL;
    }
    std::cout << "GLFW successfully initialized" << std::endl;

    // checks GLFW version
    std::cout << "GLFW compile-time version: " << GLFW_VERSION_MAJOR << GLFW_VERSION_MINOR << GLFW_VERSION_REVISION << std::endl;
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    std::cout << "GLFW run-time version: " << major << minor << revision << std::endl;

    // GLFW window specifications 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // initializes opengl version as 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // program is run in core-profile 

    // Creates a window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(in_screenWidth, in_screenHeight, "OpenGL 3D Renderer", NULL, NULL);
    if (window == NULL) {
        std::cout << "ERROR: failed to create valid OpenGL window using GLFW" << std::endl;
        glfwTerminate();
        return NULL;
    }
    std::cout << "GLFW window created" << std::endl;

    // Makes the window context current
    glfwMakeContextCurrent(window);
    std::cout << "OpenGL context created and initialized" << std::endl;


    // Initialize GLEW (after valid OpenGL context has been created) to load all OpenGl function pointers // --------------------
    if (glewInit() != GLEW_OK) {
        std::cout << "Error: glewInit() did not return GLEW_OK" << std::endl;
    }

    std::cout << "GLEW initialized" << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    return window; // on success
}