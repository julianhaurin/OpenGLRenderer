// Texture functionality - Julian Haurin - 2/4/23

#pragma once

#include <iostream>

#include <GL/glew.h>

// loads texture data into provided texture ID(?)
int loadTexture(unsigned int& in_textureData, const char* in_filePath);
