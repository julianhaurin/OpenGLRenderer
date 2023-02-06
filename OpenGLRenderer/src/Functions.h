#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Sets up opengl - intializes GLFW window and OpenGl context, loads glew function pointers //
GLFWwindow* setupOpenGL(const unsigned int in_screenWidth, const unsigned int in_screenHeight);
