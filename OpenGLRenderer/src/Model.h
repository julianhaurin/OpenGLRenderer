// Model header file - Julian Haurin - 2/3/23
// Sources: https://vulkan-tutorial.com/Loading_models ; https://github.com/tinyobjloader/tinyobjloader

#pragma once

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include <tiny_obj_loader.h>

#include "Vertices.h"


class Model
{

public:

	std::vector<tinyobj::real_t> m_vertexData = {};
	std::vector<tinyobj::index_t> m_indexData = {};

	unsigned int m_numOfVertices = 0;

	// not member intializer list - prolly fine
	Model();

	// loads a mesh given a file path into the proper Model class members 
	void LoadModel(const char* in_filePath);

	// loads a mesh given a file path and the proper VAO, VBO and EBO 
	// unsigned int LoadModel_Deprecated(unsigned int& in_VAO, unsigned int& in_VBO, unsigned int& in_EBO, const char* in_filePath);

private:

};
