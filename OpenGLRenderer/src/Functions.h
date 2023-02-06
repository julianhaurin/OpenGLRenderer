#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Sets up opengl - intializes GLFW window and OpenGl context, loads glew function pointers //
GLFWwindow* setupOpenGL(const unsigned int in_screenWidth, const unsigned int in_screenHeight);

// Callback functions //

// recalculates the viewport dimensions each time the OpenGl GLFW window is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

// handles GLFW errors using glfwSetErrorCallback
void glfwErrorCallack(int code, const char* description);