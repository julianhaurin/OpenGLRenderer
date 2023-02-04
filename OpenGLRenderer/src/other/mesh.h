// Mesh class - 1/31/23

#pragma once

// #include <iostream>
#include <vector>
#include <string>

#include <GL/glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "shader.h"

// Vertex data // 
struct Vertex {

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;

};

// Texture data //
struct Texture {

	unsigned int ID;
	std::string type;
	std::string path;
	
};

// Mesh class //
class Mesh {

	public:
		// mesh data
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		// constructor
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {

			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			setupMesh();

		}

		// draws mesh
		void Draw(Shader& shader) {

			// diffuse vs specular textures
			unsigned int diffuseNum = 1;
			unsigned int specularNum = 1;

			// handles arbitrary number of diffuse and specular textures
			for (int i = 0; i < textures.size(); i++) {
				
				glActiveTexture(GL_TEXTURE0 + i);

				// retrieves the texure number
				std::string number;
				std::string name = textures[i].type;
				
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNum++);
				else if (name == "texture_specular")
					number = std::to_string(specularNum++);

				shader.setInt(("material." + name + number).c_str(), i);
				glBindTexture(GL_TEXTURE_2D, textures[i].ID);

			}

			// renders the mesh
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
			glBindVertexArray(0);

		}

	private:
		unsigned int VAO, VBO, EBO;

		// initializes buffers
		void setupMesh() {

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, EBO);
			glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

			// vertex normals
			glEnableVertexAttribArray(1);									// offset is a preprocessor directive
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

			// vertex texture coordinates
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		}

};