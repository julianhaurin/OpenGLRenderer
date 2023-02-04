// fragment shader - 1/14/22
 
// version specs
#version 330 core
 
// input data
in vec3 color_outV; // from vertex shader, same name and type
// in vec2 texCoord_outV;
 
// output data
out vec4 FragColor;

// uniforms
uniform sampler2D texture1;
uniform sampler2D texture2;
 
// entry point
void main()
{

  // multiplying texture and color to combine                    
  FragColor = vec4(color_outV, 1.0f); // * texture(texture1, texCoord_outV);
 
}
