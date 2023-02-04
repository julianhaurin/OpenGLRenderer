// OpenGl renderer (eventually) - 1/14/23 - Julian Haurin
// Following learnopengl.com tutorial

#include <iostream>
#include <cstdint>

#include <GL/glew.h>    // retrieves OpenGL function pointers from the graphics card
#include <GLFW/glfw3.h> // creates cross-platform OpenGL context, handles user input

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> // image loading library

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Vertices.h"

// forward declarations
void processInput(GLFWwindow* window);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void glfwErrorCallack(int code, const char* description);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

// constants
const static uint32_t SCREEN_WIDTH = 800;
const static uint32_t SCREEN_HEIGHT = 600;

// file paths
const char MODEL_PATH[] = "./assets/models/monkey.obj";
const char VERTEX_SHADER_PATH[] = "./shaders/vertexShader.vs";
const char FRAGMENT_SHADER_PATH[] = "./shaders/fragmentShader.fs";

float static deltaTime = 0.0f; // Time between the current frame and the last frame
float static lastFrame = 0.0f; // Time duration of last frame

// camera data
glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // positive z-axis is through screen
Camera camera = Camera(cameraPos, worldUp);

// model data
Model modelObj = Model();

// initial mouse positions
float lastMouseX = 400;
float lastMouseY = 300;

bool firstMouseInput = true; // smooths initial mouse movement


int main() {

    // SETUP // -----------------------------------------------------------------------------------------------------------------
    GLFWwindow* window;

    // Initializes and configures the GLFW library and OpenGL context // --------------------------------------------------------
    if (glfwInit() == GLFW_FALSE) {
        std::cout << "ERROR: GLFW failed to initialize" << std::endl;
        return -1;
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
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL 3D Renderer", NULL, NULL);   
    if (window == NULL) {
        std::cout << "ERROR: failed to create valid OpenGL window using GLFW" << std::endl;
        glfwTerminate();
        return -1;
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
    // --------------------------------------------------------------------------------------------------------------------------
     
    // Callback functions, handled by GLFW, called whenever necessary //

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // recalculates viewport each time window is resized
    glfwSetErrorCallback(glfwErrorCallack); // handles GLFW errors 
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // mouse settings
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hides and captures cursor


    // Create and compile the shader program using the custom Shader class // ---------------------------------------------------
    Shader shaderProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    

    // Initializing and configuring VBO, EBO, and VAO // ------------------------------------------------------------------------

    // vertex attribute object (VAO)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // vertex buffer object (VBO)
    unsigned int VBO;
    glGenBuffers(1, &VBO); // generates buffer id

    // element buffer object (EBO)
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // loads vertex data
    modelObj.LoadModel(MODEL_PATH);

    // configuring provided VAO, VBO, and EBO //

    glBindVertexArray(VAO); // binds vertex array object

    tinyobj::real_t vertexDataArr[100000];
    tinyobj::index_t indexDataArr[10000];

    // FIX // -----------------------------------------------------------

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

    // std::copy(modelObj.m_vertexData.begin(), modelObj.m_vertexData.end(), vertexDataArr);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // binds VBO to an array buffer (which is the VBO buffer type)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexDataArr), vertexDataArr, GL_STATIC_DRAW); // copies vertex data into buffer's memory

    // configuring EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexDataArr), indexDataArr, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);


    // Textures // --------------------------------------------------------------------------------------------------------------
 
    // generating textures
    unsigned int texture1, texture2;

    // texture1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // setting texture wrapping parameters, uses s, t, r coordinate system
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 2D textures will repeat mirrored

    // determines point (nearest-neighbor) filtering and bilinear filtering for textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // when scaling down, OpenGL will use bilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // when scaling up, OpenGL will use bilinear filtering

    // mipmap configuring
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image from disk using stbi
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* textureData1 = stbi_load("./assets/textures/drunkCat.jpg", &width, &height, &nrChannels, 0);

    // checks that the image loaded successfully
    if (!textureData1) {
        std::cout << "Error: texture1 image failed to load" << std::endl;

    }
    else {

        std::cout << "Loaded texture data successfully" << std::endl;
        // load data from stbi into OpenGL
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // pixel transfer configuring
        // Allocates mutable storage for a mipmap level of the bound texture object
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData1); // pixel unpack operation
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    // frees image memory
    stbi_image_free(textureData1);

    // texture2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* textureData2 = stbi_load("./assets/textures/awesomeFace.png", &width, &height, &nrChannels, 0);

    // checks that the image loaded successfully
    if (!textureData2) {
        std::cout << "Error: texture2 image failed to load" << std::endl;

    }
    else {

        std::cout << "Loaded texture data successfully" << std::endl;
        // load data from stbi into OpenGL
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // cuz it was crashing before
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData2);
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    // frees image memory
    stbi_image_free(textureData2);

    shaderProgram.use();
    //glUniform1i(glGetUniformLocation(shaderProgram.ID, "texture1"), 0); // sets texture to 0
    //shaderProgram.setInt("texture2", 1); // same thing, but with Shader class
    
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

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // VAO stores EBO too, so binding VAO also binds corresponding EBO
        glBindVertexArray(VAO); // not entirely necessary with only one VAO, but organized

        shaderProgram.use(); // activates program object

        // Transformations with Linear Algebra // Vclip = Mprojection * Mview * Mmodel * Mlocal // ------------------------------

        // initialize space matrices
        glm::mat4 model = glm::mat4(1.0f); // model matrix
        glm::mat4 view = glm::mat4(1.0f); // view matrix
        glm::mat4 projection = glm::mat4(1.0f); // projection matrix

        // calculate model matrix
        // model = glm::rotate(model, currentFrame * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
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

// recalculates the viewport dimensions each time the OpenGl GLFW window is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {

    // glViewPort is used to transform the 2D coordinates OpenGL processes to points on your screen (behind the scene)
    glViewport(0, 0, width, height);

}

// handles GLFW errors using glfwSetErrorCallback
void glfwErrorCallack(int code, const char* description) {

    std::cout << "GLFW error reported: " << description << std::endl;

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

