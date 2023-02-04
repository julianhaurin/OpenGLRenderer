// Model class - Julian Haurin - 2/4/23


// constructor


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