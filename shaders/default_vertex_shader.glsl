#version 330 core

layout(location = 0) in vec3 aPos;  // Vertex position

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // Output a simple color based on vertex position
    vertexColor = (aPos + vec3(1.0)) * 0.5;
}
