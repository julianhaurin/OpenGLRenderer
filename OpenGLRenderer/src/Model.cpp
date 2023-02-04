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

// DEPRECATED - ERRORS // -------------------------------------------------------------------------------------------------------

/* 
// loads a mesh given a file path and the proper VAO, VBO and EBO 
unsigned int Model::LoadModel_Deprecated(unsigned int& in_VAO, unsigned int& in_VBO, unsigned int& in_EBO, const char* in_filePath) {

	// returns the number of vertices, necessary for glDrawElements()
	unsigned int out_numOfVertices = 0;

	// vertex data filled with data from obj file
	std::vector<float> vertices;
	std::vector<float> indices;

	// holds obj positions, normals, and texture coordinates in attrib.vertices, attrib.normals, and attrib.texcoords vectors
	tinyobj::attrib_t attrib;

	std::vector<tinyobj::shape_t> shapes; // contains all separate objects and their faces
	std::vector<tinyobj::material_t> materials;

	std::string warn, err;

	// loads model into tinyobjloader data structures
	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, in_filePath);

	// warning and error handling
	if (!warn.empty()) {
		std::cout << "TinyObjLoader warning: " << warn << std::endl;
	}

	if (!err.empty()) {
		std::cerr << "TinyObjLoader error: " << warn << std::endl;
	}

	if (!success) {
		std::cout << "ERROR: tinyobjloader failed to load obj file with LoadObj() " << std::endl;
		return 0;
	}

	// combines all of the faces in the obj file into a single model by iterating through each shape
	for (const auto& shape : shapes) {

		// triangulation feature enabled by default, three vertices per face
		for (const auto& index : shape.mesh.indices) {

			// fill vertices vector with data //

			// vertex position, assuming unique vertices for now
			vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
			vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
			vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

			// vertex color
			vertices.push_back(0.5f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);

			// texture coordinates
			//vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
			//vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);

			indices.push_back(indices.size());

			out_numOfVertices++;

		}

	}

	// configuring provided VAO, VBO, and EBO //

	glBindVertexArray(in_VAO); // binds vertex array object

	glBindBuffer(GL_ARRAY_BUFFER, in_VBO); // binds VBO to an array buffer (which is the VBO buffer type)
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices[0], GL_STATIC_DRAW); // copies vertex data into buffer's memory

	// configuring EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, in_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	std::cout << "DEPRECATED FUNCTION: Loaded OBJ and configured VAO, VBO, and EBO values" << std::endl;

	return out_numOfVertices;

}

*/

// ------------------------------------------------------------------------------------------------------------------------------