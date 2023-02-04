// vertex shader - 1/14/22
 
// version specs
#version 330 core
 
// location metadata defines how the vertex data is organized and input data

layout (location = 0) in vec3 position; // the position variable has attribute position 0
layout (location = 1) in vec3 colorRGB;
layout (location = 2) in vec2 textureCoord; 

 
// output data
out vec3 color_outV; // specifies color output for the fragment shader
out vec2 texCoord_outV;

// uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
 
// entry point
void main() 
{

	// matrix multiplication is backwards
	gl_Position = projection * view * model * vec4(position, 1.0f); // predefined variable, uses 'in' variable 'aPos'
	color_outV = colorRGB; // output color
	texCoord_outV = textureCoord;

 
}

