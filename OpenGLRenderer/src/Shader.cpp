// Shader class (.cpp) - Julian Haurin - 2/6/23 // 

#include "Shader.h"

// constructor to read and build the shader
Shader::Shader(const char* vertexPath, const char* fragmentPath) {

	// retrieve vertex and fragment shader source code from file paths //
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile; // vertex shader
	std::ifstream fShaderFile; // fragment shader

	// ensures that ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// opens files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream;
		std::stringstream fShaderStream;

		// read file buffer content into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// close file handlers
		vShaderFile.close();
		fShaderFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	}
	catch (std::ifstream::failure e) {
		std::cout << "[J] ERROR: shader file not successfully read" << std::endl;
	}

	// convert to c-strings
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// compile shaders //
	unsigned int vertexID;
	unsigned int fragmentID;

	int vSuccess;
	int fSuccess;
	int sSuccess; // shader program success

	char vInfoLog[512];
	char fInfoLog[512];
	char sInfoLog[512]; // shader program info log

	// vertex shader compilation
	vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vShaderCode, NULL);
	glCompileShader(vertexID);

	// checks vertex shader success and retrieves any error logs
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &vSuccess);

	// if the compilation failed, prints error log
	if (!vSuccess) {
		glGetShaderInfoLog(vertexID, 512, NULL, vInfoLog);
		std::cout << "[J] ERROR: failed to compile vertex shader: " << vInfoLog << std::endl;

	}

	// fragment shader compilation
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fShaderCode, NULL);
	glCompileShader(fragmentID);

	// checks fragment shader success and retrieves and error logs
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &fSuccess);

	// if the compilation failed, prints error log
	if (!fSuccess) {
		glGetShaderInfoLog(fragmentID, 512, NULL, fInfoLog);
		std::cout << "[J] ERROR: failed to compile fragment shader: " << fInfoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexID);
	glAttachShader(ID, fragmentID);
	glLinkProgram(ID);

	// prints any linking errors
	glGetProgramiv(ID, GL_LINK_STATUS, &sSuccess);
	if (!sSuccess) {
		glGetProgramInfoLog(ID, 512, NULL, sInfoLog);
		std::cout << "[J] ERROR: failed to link shader program: " << sInfoLog << std::endl;

	}
	std::cout << "[J] Shader program successfully linked! " << std::endl;

	// deletes unnecessary shaders
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);

}

// use the shader
void Shader::use() {
	glUseProgram(ID);
}

// utility uniform functions - set shader uniform variables like glGetUniformLocation() and glUniformMatrix4fv()
void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}