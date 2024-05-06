#version 330 core

layout(location = 0) in vec3 aPos;

out vec2 MyCoord;

void main(){
    gl_Position = vec4(aPos, 1.0f);
    MyCoord = vec2(aPos.x, aPos.y); // Example of custom coordinate calculation
}