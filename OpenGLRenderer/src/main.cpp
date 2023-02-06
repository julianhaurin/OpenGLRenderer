// OpenGl renderer (eventually) - 1/14/23 - Julian Haurin
// Following: learnopengl.com tutorial ; vulkan-tutorial.com/Loading_models ; github.com/tinyobjloader/tinyobjloader

// Notes: -----------------------------------------------------------------------------------------------------------------------
// 
// setUpOpengl() - window pointer to local/stack data (if extracted to separate file)
// how to extract callback functions when internal camera functionality is required
// add texture funtionality
// how do separate functions work that require OpenGL context - how to make secure?
// fix texture loading function - stbi #define throwing errors - check data types reference/pointers - maybe problematic
// add texture mtl file input in model class
// fix naming convenctions (capitilization, in_, m_, etc.)
// not sure how to handle #defines with stbi and tinyobj - throw a lot of errors in past
// personal console messages should start with [J] and erros should start with [J] ERROR: 
// weird error with tinyobjloader loading textures - keeps saying failed to load from "path" but path always has "\" at the end
// for now, textures (.mtl) have to stay in same folder as model bc of above error
// mtl file name is in obj file, had to change that, mightve fixed bug
// fix ebo so i can use gldrawelements (looks weird rn 2/6/23)
// think more about ood and model class - maybe add VBO VAO EBO, and texture info too
// fix EBO vs index shit - glDrawElements not working correctly
// add more flexibility in model ConfigureModel - stride, vertex type, etc. also in texture loading functions
// return success int in model methods, not void?
// #define STB_IMAGE_IMPLEMENTATION being weird with linker too - gotta figure this shit out
// use cstdint everywhere?
// check if texture binding and other shit with shaders is working
// try multiple models!
// check warnings - lots from stbi and tinyobjloader too?
//
// ------------------------------------------------------------------------------------------------------------------------------

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
#include "Functions.h"


// Forward Declarations - require camera class so kept in main file (for now) //

void processInput(GLFWwindow* window);
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
const char VERTEX_SHADER_PATH[] = "./shaders/vertexShader.vs";
const char FRAGMENT_SHADER_PATH[] = "./shaders/fragmentShader.fs";

const char MODEL_PATH[] = "./assets/models/vikingRoom.obj";
const char TEXTURE_PATH[] = "./assets/textures/vikingRoom.mtl";

// initial mouse positions
float lastMouseX = 400;
float lastMouseY = 300;

bool firstMouseInput = true; // smooths initial mouse movement

int main() {

    // Setup // -----------------------------------------------------------------------------------------------------------------
    GLFWwindow* window = setupOpenGL(SCREEN_WIDTH, SCREEN_HEIGHT);

    if (!window) {
        std::cout << "[J] ERROR: failed to setup OpenGL with setupOpenGL() " << std::endl;
    }

    // Callback functions, handled by GLFW, called whenever necessary // --------------------------------------------------------

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // recalculates viewport each time window is resized
    glfwSetErrorCallback(glfwErrorCallack); // handles GLFW errors 
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hides and captures cursor

    // Create and compile the shader program using the custom Shader class // ---------------------------------------------------
    Shader shaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

    // Rendering Data - Initializing and configuring VBO, EBO, and VAO // -------------------------------------------------------
    Model vikingRoom = Model();

    vikingRoom.LoadModel(MODEL_PATH); // loads vertex data
    vikingRoom.ConfigureModel(); // configures vertex data
    vikingRoom.LoadTexture(TEXTURE_PATH); // loads texture data
    vikingRoom.ConfigureTexture(); // configures texture data
    vikingRoom.BindModel(); // binds data

    // --------------------------------------------------------------------------------------------------------------------------

    shaderProgram.use();
    shaderProgram.setInt("texture", 0); // sets texture ID data(?) using Shader class

    glEnable(GL_DEPTH_TEST);

    // Render Loop // -----------------------------------------------------------------------------------------------------------

    float count = 0;
    std::cout << "[J] Starting render loop... " << std::endl;

    // loops until the user closes the window
    while (!glfwWindowShouldClose(window)) { // a render loop is also called a frame

        // time calculations
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // processes inputs at the beginning of each loop
        processInput(window);

        // Render // ------------------------------------------------------------------------------------------------------------

        glClearColor(0.3f, 0.06f, 0.05f, 1.0f); // background color
        glClear(GL_COLOR_BUFFER_BIT); // clears window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears z-buffer

        //// bind textures on corresponding texture units
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture);

        shaderProgram.use(); // activates program object

        // Transformations with Linear Algebra // Vclip = Mprojection * Mview * Mmodel * Mlocal // ------------------------------

        // initialize space matrices
        glm::mat4 model = glm::mat4(1.0f); // model matrix
        glm::mat4 view = glm::mat4(1.0f); // view matrix
        glm::mat4 projection = glm::mat4(1.0f); // projection matrix

        // calculate model matrix
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // z
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // y
        shaderProgram.setMat4("model", model);

        // calculate view matrix
        view = camera.calculateViewMatrix();
        shaderProgram.setMat4("view", view);

        // calculate projection matrix
        projection = glm::perspective(glm::radians(camera.cameraZoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        shaderProgram.setMat4("projection", projection);

        // calculate transformation matrix
        glm::mat4 transformation = glm::mat4(1.0f);
        transformation = glm::rotate(transformation, currentFrame, glm::vec3(0.0f, 0.0f, 1.0f));

        // Initialize rendering pipeline // -------------------------------------------------------------------------------------

        vikingRoom.BindModel();
        // glDrawElements(GL_TRIANGLES, modelObj.m_vertexData.size(), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, vikingRoom.m_vertexData.size());

        // Cleanup // -----------------------------------------------------------------------------------------------------------

        glfwSwapBuffers(window); // swaps front and back buffers

        // poll for and process events, handles callback functions (?)
        glfwPollEvents(); // checks if any events are triggered (keyboard, mouse movements, etc.) and calls necessary functions

        count++;

    }

    std::cout << "Frame count: " << count << std::endl;

    // CLEANUP // ---------------------------------------------------------------------------------------------------------------

    vikingRoom.DeleteModelData();

    glfwDestroyWindow(window);
    glfwTerminate(); // cleans up, restores any global settings changes
    return 0;

    // --------------------------------------------------------------------------------------------------------------------------
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

