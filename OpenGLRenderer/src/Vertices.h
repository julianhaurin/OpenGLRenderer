// Vertex arrays and cube positions - Julian Haurin - 2/3/23 //

#pragma once

#include <vector>

#include <glm.hpp>

// normalized device coordinates (NDC) are transformed to screen-space coordinates via the viewport transform
std::vector<tinyobj::real_t> squareVertices = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   //1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   //1.0f, 0.0f, // bottom right
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   //0.0f, 1.0f  // top left 
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   //1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   //0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   //0.0f, 1.0f  // top left 
};

tinyobj::real_t cubeVertices[] = {
    // positions          // colors          // texture coords
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  //0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  //1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  //1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  //1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  //0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  //0.0f, 0.0f,
                                             //
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  //0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  //1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  //1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  //1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  //0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  //0.0f, 0.0f,
                                             //
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  //1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  //1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  //0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  //0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  //0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  //1.0f, 0.0f,
                                             //
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  //1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  //1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  //0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  //0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  //0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  //1.0f, 0.0f,
                                             //
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  //0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  //1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  //1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  //1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  //0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  //0.0f, 1.0f,
                                             //
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  //0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  //1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  //1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  //1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  //0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  //0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

// indices for indexed drawing
unsigned int v_indices[] = {
    0, 1, 2, // first triangle
    1, 2, 3  // second triangle
};
