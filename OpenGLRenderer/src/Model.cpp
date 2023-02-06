// Model class - Julian Haurin - 2/4/23

// constructor
#define TINYOBJLOADER_IMPLEMENTATION
#include "Model.h"

Model::Model() {


}

// loads a mesh given a file path into the proper Model class members 
void Model::LoadModel(const char* in_filePath) {

	tinyobj::ObjReader reader;

	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./assets/textures/"; // Path to material files

	// reads 
	if (!reader.ParseFromFile(in_filePath, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "[J] ERROR: failed to load obj file: " << reader.Error();
		}
		exit(1);
	}

	// handles warning 
	if (!reader.Warning().empty()) {
		std::cout << "[J] WARNING: from tinyobjloader: " << reader.Warning();
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

				tinyobj::index_t index = shapes[shape].mesh.indices[index_offset + vertex];
				m_indexData.push_back(index);

				// vertex position
				m_vertexData.push_back(attrib.vertices[3 * (float)index.vertex_index]); // + 0
				m_vertexData.push_back(attrib.vertices[3 * (float)index.vertex_index + 1]);
				m_vertexData.push_back(attrib.vertices[3 * (float)index.vertex_index + 2]);

				// vertex colors
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
				if (index.texcoord_index >= 0) {
					m_vertexData.push_back(attrib.texcoords[2 * (float)index.texcoord_index + 0]);
					m_vertexData.push_back(attrib.texcoords[2 * (float)index.texcoord_index + 1]);
				}

			}

			index_offset += faceVertices;
			// shapes[shape].mesh.material_ids[face]; (?)

		}

	}

	std::cout << "[J] Loaded OBJ " << std::endl;

	return;

}

// (after loading mesh data) configures VAO, VBO, and EBO
void Model::ConfigureModel() {

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // binds VBO to an array buffer (which is the VBO buffer type)
	glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(float), &m_vertexData[0], GL_STATIC_DRAW); // copies vertex data into buffer's memory

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// configuring EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexData.size() * sizeof(float), &m_indexData[0], GL_STATIC_DRAW);

}

// loads texture data into model
void Model::LoadTexture(const char* in_texturePath) {

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// load image from disk using stbi
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* textureData = stbi_load(in_texturePath, &width, &height, &nrChannels, 0);

	// checks that the image loaded successfully
	if (!textureData) {
		std::cout << "[J] ERROR: texture1 image failed to load" << std::endl;
		return;

	}
	else {

		std::cout << "[J] Loaded texture data successfully" << std::endl;
		// load data from stbi into OpenGL
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // pixel transfer configuring
		// Allocates mutable storage for a mipmap level of the bound texture object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData); // pixel unpack operation
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	// frees image memory
	stbi_image_free(textureData);

}

// configures texture settings
void Model::ConfigureTexture() {

	// setting texture wrapping parameters, uses s, t, r coordinate system
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // 2D textures will repeat mirrored

	// determines point (nearest-neighbor) filtering and bilinear filtering for textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // when scaling down, OpenGL will use bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // when scaling up, OpenGL will use bilinear filtering

	// mipmap configuring
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

// binds all necessary data in model
void Model::BindModel() {

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

}

// deallocates model data once the object is no longer in use
void Model::DeleteModelData() {

	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);

}