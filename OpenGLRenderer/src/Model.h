// Model header file - Julian Haurin - 2/3/23
// Sources: https://vulkan-tutorial.com/Loading_models ; https://github.com/tinyobjloader/tinyobjloader

#pragma once

#include <iostream>
#include <vector>
#include <cstdint>

#include <GL/glew.h>

#include <tiny_obj_loader.h>

#include <stb_image.h>


class Model
{

public:

	// should I be zeroing variables?
	uint32_t m_VAO = 0; // Vertex Array Object (VAO)
	uint32_t m_VBO = 0; // Vertex Buffer Object (VBO)
	uint32_t m_EBO = 0; // Element Buffer Object (EBO)

	uint32_t m_texture = 0; // texture ID

	std::vector<tinyobj::real_t> m_vertexData = {};
	std::vector<tinyobj::index_t> m_indexData = {};

	Model();

	// loads a mesh given a file path into the proper Model class members 
	void LoadModel(const char* in_filePath);

	// (after loading mesh data) configures VAO, VBO, and EBO
	void ConfigureModel();

	// loads texture data into model
	void LoadTexture(const char* in_texturePath);

	// configures texture settings
	void ConfigureTexture();

	// binds all necessary data in model
	void BindModel();

	// deallocates model data once the object is no longer in use
	void DeleteModelData();


private:

};
