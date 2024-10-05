#version 430 core

layout (location = 0) in vec3 aPos; // 3D position of each vertex
out vec3 texCoord;  // UV coordinates for the fragment shader

void main() {
    texCoord = aPos * 0.5 + 0.5;  // Pass the vertex positions directly as UV coordinates
    gl_Position = vec4(aPos.xy, 0.0, 1.0);  // 2D rendering, discard Z
}
