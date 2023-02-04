// GLFW callback functions - Julian Haurin - 2/4/23

#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Camera.h>

// recalculates the viewport dimensions each time the OpenGl GLFW window is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

// handles GLFW errors using glfwSetErrorCallback
void glfwErrorCallack(int code, const char* description);

