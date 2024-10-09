#version 430 core

in vec3 vertexColor;  // Color passed from the vertex shader
out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0);  // Set the output color
}
