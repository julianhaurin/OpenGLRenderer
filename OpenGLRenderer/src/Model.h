// Model header file - Julian Haurin - 2/3/23
// Sources: https://vulkan-tutorial.com/Loading_models ; https://github.com/tinyobjloader/tinyobjloader

#pragma once

#include <iostream>
#include <vector>

#include <GL/glew.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Vertices.h"


class Model
{

public:

	std::vector<tinyobj::real_t> m_vertexData;
	std::vector<tinyobj::index_t> m_indexData;

	unsigned int m_numOfVertices;

	// not member intializer list - prolly fine
	Model();

	// loads a mesh given a file path into the proper Model class members 
	void LoadModel(const char* in_filePath);

	// loads a mesh given a file path and the proper VAO, VBO and EBO 
	// unsigned int LoadModel_Deprecated(unsigned int& in_VAO, unsigned int& in_VBO, unsigned int& in_EBO, const char* in_filePath);

private:

};

Model::Model()
{
	// not member intializer list - prolly fine
	m_vertexData = {};
	m_indexData = {};

	m_numOfVertices = 0;

}

// loads a mesh given a file path into the proper Model class members 
void Model::LoadModel(const char* in_filePath) {

	tinyobj::ObjReader reader;

	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./assets/textures/"; // Path to material files

	// reads 
	if (!reader.ParseFromFile(in_filePath, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "Error loading obj file: " << reader.Error();
		}
		exit(1);
	}

	// handles warning 
	if (!reader.Warning().empty()) {
		std::cout << "Warning from tinyobjloader: " << reader.Warning();
	}

	// loads data into attrib
	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	// loops over shapes
	for (size_t shape = 0; shape < shapes.size(); shape++) {

		size_t index_offset = 0;

		// loops over faces
		for (size_t face = 0; face < shapes[shape].mesh.num_face_vertices.size(); face++) {
			size_t faceVertices = size_t(shapes[shape].mesh.num_face_vertices[face]);

			// loops over vertices
			for (size_t vertex = 0; vertex < faceVertices; vertex++) {

				m_numOfVertices++;

				tinyobj::index_t index = shapes[shape].mesh.indices[index_offset + vertex];
				m_indexData.push_back(index);

				// vertex position
				// std::cout << "Loading position data... " << std::endl;
				m_vertexData.push_back(attrib.vertices[3 * index.vertex_index]); // + 0
				m_vertexData.push_back(attrib.vertices[3 * index.vertex_index + 1]);
				m_vertexData.push_back(attrib.vertices[3 * index.vertex_index + 2]);

				// vertex colors
				// std::cout << "Loading color data... " << std::endl;
				m_vertexData.push_back(0.1f);
				m_vertexData.push_back(0.2f);
				m_vertexData.push_back(0.4f);

				//m_vertexData.push_back(attrib.colors[3 * size_t(index.vertex_index) + 0]);
				//m_vertexData.push_back(attrib.colors[3 * size_t(index.vertex_index) + 1]);
				//m_vertexData.push_back(attrib.colors[3 * size_t(index.vertex_index) + 2]);

				//// normal attributes
				//if (index.normal_index >= 0) {
				// 	std::cout << "Loading normal data... " << std::endl;
				//	m_vertexData.push_back(attrib.normals[3 * size_t(index.normal_index) + 0]);
				//	m_vertexData.push_back(attrib.normals[3 * size_t(index.normal_index) + 1]);
				//	m_vertexData.push_back(attrib.normals[3 * size_t(index.normal_index) + 2]);
				//}

				// texture coordinates
				//if (index.texcoord_index >= 0) {
				//	std::cout << "Loading texture data... " << std::endl;
				//	m_vertexData.push_back(attrib.texcoords[2 * size_t(index.texcoord_index) + 0]);
				//	m_vertexData.push_back(attrib.texcoords[2 * size_t(index.texcoord_index) + 1]);
				//}

			}

			index_offset += faceVertices;
			shapes[shape].mesh.material_ids[face];

		}

	}

	// prints data
	/*for (int i = 0; i < m_vertexData.size(); i++) {
		std::cout << m_vertexData[i] << " ";
	}*/

	// in_vertexData = m_vertexData;

	std::cout << "Loaded OBJ " << std::endl;

	return;

}