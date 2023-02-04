// OpenGl renderer (eventually) - 1/14/23 - Julian Haurin
// Following: learnopengl.com tutorial ; vulkan-tutorial.com/Loading_models ; github.com/tinyobjloader/tinyobjloader

// Notes:
// 
// fix model loading
// separate model.h into model.cpp
// setUpOpengl() - window pointer to local/stack data (if extracted to separate file)
// how to extract callback functions when internal camera functionality is required
// add texture funtionality
// how do separate functions work that require OpenGL context - how to make secure?
// fix texture loading function - stbi #define throwing errors - check data types reference/pointers - maybe problematic
// add texture mtl file input in model class
// fix naming convenctions (capitilization, in_, m_, etc.)
// not sure how to handle #defines with stbi and tinyobj - throw a lot of errors in past
// fix Model.h and Model.cpp - right now (2/4/23) all code is in .h to avoid linking erros (idk y)

#include <iostream>
#include <cstdint>

#include <GL/glew.h>    // retrieves OpenGL function pointers from the graphics card
#include <GLFW/glfw3.h> // creates cross-platform OpenGL context, handles user input

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <tiny_obj_loader.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Textures.h"
#include "Vertices.h"
#include "CallbackFunctions.h"


// Forward Declarations //
GLFWwindow* setupOpenGL();
void processInput(GLFWwindow* window);

// these callbacks require access to a global Camera, so kept in main file (for now)
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

// Constants //
const static uint32_t SCREEN_WIDTH = 800;
const static uint32_t SCREEN_HEIGHT = 600;

float static deltaTime = 0.0f; // Time between the current frame and the last frame
float static lastFrame = 0.0f; // Time duration of last frame

// camera data
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // positive z-axis is through screen
Camera camera = Camera(cameraPos, worldUp);

// file paths
const char MODEL_PATH[] = "./assets/models/monkey.obj";
const char VERTEX_SHADER_PATH[] = "./shaders/vertexShader.vs";
const char FRAGMENT_SHADER_PATH[] = "./shaders/fragmentShader.fs";

// initial mouse positions
float lastMouseX = 400;
float lastMouseY = 300;

bool firstMouseInput = true; // smooths initial mouse movement

int main() {

    // SETUP // -----------------------------------------------------------------------------------------------------------------
    GLFWwindow* window = setupOpenGL();

    if (!window) {
        std::cout << "ERROR: failed to setup OpenGL with setupOpenGL() " << std::endl;
    }

     
    // Callback functions, handled by GLFW, called whenever necessary //---------------------------------------------------------

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // recalculates viewport each time window is resized
    glfwSetErrorCallback(glfwErrorCallack); // handles GLFW errors 
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // mouse settings
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hides and captures cursor

    // Create and compile the shader program using the custom Shader class // ---------------------------------------------------
    Shader shaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    // Rendering Data - Initializing and configuring VBO, EBO, and VAO // -------------------------------------------------------

    // loading model data //
    Model modelObj = Model();
    
    unsigned int VAO; // vertex attribute object (VAO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); // binds vertex array object

    unsigned int VBO; // vertex buffer object (VBO)
    glGenBuffers(1, &VBO); // generates buffer id

    unsigned int EBO; // element buffer object (EBO)
    glGenBuffers(1, &EBO);

    modelObj.LoadModel(MODEL_PATH); // loads vertex data


    // FIX // -------------------------------------------------------------------------------------------------------------------

    tinyobj::real_t vertexDataArr[100000];
    tinyobj::index_t indexDataArr[10000];

    for (int i = 0; i < modelObj.m_vertexData.size(); i++) {
        if (i >= 100000)
            break;
        vertexDataArr[i] = modelObj.m_vertexData[i];
    }

    for (int j = 0; j < modelObj.m_indexData.size(); j++) {
        if (j >= 10000)
            break;
        indexDataArr[j] = modelObj.m_indexData[j];
    }

    // --------------------------------------------------------------------------------------------------------------------------

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // binds VBO to an array buffer (which is the VBO buffer type)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexDataArr), vertexDataArr, GL_STATIC_DRAW); // copies vertex data into buffer's memory

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // glEnableVertexAttribArray(2);

    // configuring EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexDataArr), indexDataArr, GL_STATIC_DRAW);


    // Textures // --------------------------------------------------------------------------------------------------------------
 
    // generating textures
    unsigned int texture1;

    if (loadTexture(texture1) != 0) {
        std::cout << "ERROR: failed to load texture data with loadTexture() " << std::endl;
    }

    shaderProgram.use();
    shaderProgram.setInt("texture2", 1); // sets texture ID data(?) using Shader class
    
    glEnable(GL_DEPTH_TEST);

    // Render Loop // -----------------------------------------------------------------------------------------------------------

    float count = 0;

    // loops until the user closes the window
    while (!glfwWindowShouldClose(window)) { // a render loop is also called a frame

        // time calculations
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // processes inputs at the beginning of each loop
        processInput(window);

        // Render // ------------------------------------------------------------------------------------------------------------

        glClearColor(0.1f, 0.02f, 0.05f, 1.0f); // background color
        glClear(GL_COLOR_BUFFER_BIT); // clears window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears z-buffer

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // VAO stores EBO too, so binding VAO also binds corresponding EBO
        glBindVertexArray(VAO); // not entirely necessary with only one VAO, but organized

        shaderProgram.use(); // activates program object

        // Transformations with Linear Algebra // Vclip = Mprojection * Mview * Mmodel * Mlocal // ------------------------------

        // initialize space matrices
        glm::mat4 model = glm::mat4(1.0f); // model matrix
        glm::mat4 view = glm::mat4(1.0f); // view matrix
        glm::mat4 projection = glm::mat4(1.0f); // projection matrix

        // calculate model matrix
        model = glm::rotate(model, currentFrame * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // calculate view matrix
        view = camera.calculateViewMatrix();
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // calculate projection matrix
        projection = glm::perspective(glm::radians(camera.cameraZoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        int projlLoc = glGetUniformLocation(shaderProgram.ID, "projection");
        glUniformMatrix4fv(projlLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // calculate transformation matrix
        glm::mat4 transformation = glm::mat4(1.0f);
        transformation = glm::rotate(transformation, currentFrame, glm::vec3(0.0f, 0.0f, 1.0f));

        // Initialize rendering pipeline // -------------------------------------------------------------------------------------
        //glDrawElements(GL_TRIANGLES, modelObj.m_numOfVertices, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, modelObj.m_numOfVertices);

        // Cleanup // -----------------------------------------------------------------------------------------------------------

        glfwSwapBuffers(window); // swaps front and back buffers

        // poll for and process events, handles callback functions (?)
        glfwPollEvents(); // checks if any events are triggered (keyboard, mouse movements, etc.) and calls necessary functions

        count++;

    }

    // CLEANUP // ---------------------------------------------------------------------------------------------------------------

    // deallocates resources once the program has completed
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    std::cout << "Frame count: " << count << std::endl;

    glfwDestroyWindow(window);
    glfwTerminate(); // cleans up, restores any global settings changes
    return 0;

    // --------------------------------------------------------------------------------------------------------------------------
}

// Sets up opengl - intializes GLFW window and OpenGl context, loads glew function pointers //
GLFWwindow* setupOpenGL() {

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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL 3D Renderer", NULL, NULL);
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

// process keyboard inputs using glfwGetKey
void processInput(GLFWwindow* window) {

    // std::cout << deltaTime << std::endl;

    // exits the window (render loop) if the 'ESC' key is being pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // otherwise == GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);

    // courtesys of Felix Armbruster on learnopengl.com
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // WASD movements
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(RIGHT, deltaTime);

    // UP DOWN movement
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(DOWN, deltaTime);

}


// handles mouse movement for calculaing Euler angles
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {

    if (firstMouseInput) {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouseInput = false;
    }

    // calculate offset of mouse position from last frame
    float xOffset = (float)xpos - lastMouseX;
    float yOffset = (float)lastMouseY - ypos;

    // update mouse positions for next frame
    lastMouseX = xpos;
    lastMouseY = ypos;

    camera.ProcessMouseMovement(xOffset, yOffset);

}

// handles scrolling by zooming in or out
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {

    camera.ProcessMouseScroll((float)yOffset);

}

