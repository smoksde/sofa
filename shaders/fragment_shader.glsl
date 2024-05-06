#version 330 core

layout(location = 0) out vec4 FragColor;

uniform vec4 u_color;

void main(){

    // Get the fragment's position in normalized device coordinates
    vec2 fragCoord = gl_FragCoord.xy / vec2(800, 800); // Assuming a resolution of 800x800

    // Define the rectangle's dimensions and position
    float left = 0.0;
    float right = 0.5;
    float bottom = 0.0;
    float top = 0.5;

    if (fragCoord.x >= left && fragCoord.x <= right && fragCoord.y >= bottom && fragCoord.y <= top) {
        // Set the fragment color to white if inside the rectangle
        FragColor = vec4(0.6, 1.0, 1.0, 1.0);
        return;
    }

    // Define the rectangle's dimensions and position
    left = 0.75;
    right = 1.0;
    bottom = 0.0;
    top = 1.0;

    if (fragCoord.x >= left && fragCoord.x <= right && fragCoord.y >= bottom && fragCoord.y <= top) {
        // Set the fragment color to white if inside the rectangle
        FragColor = vec4(0.6, 1.0, 1.0, 1.0);
        return;
    }

    // Define the rectangle's dimensions and position
    left = 0.0;
    right = 1.0;
    bottom = 0.75;
    top = 1.0;

    if (fragCoord.x >= left && fragCoord.x <= right && fragCoord.y >= bottom && fragCoord.y <= top) {
        // Set the fragment color to white if inside the rectangle
        FragColor = vec4(0.6, 1.0, 1.0, 1.0);
        return;
    }

    // Set the fragment color to black if outside the rectangle
    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}