#version 330 core

layout(location = 0) out vec4 FragColor;

uniform vec4 u_color;

void main(){

    // f_color = u_color;

    // Define the rectangle's dimensions and position
    float left = 0.0;
    float right = 0.5;
    float bottom = -0.5;
    float top = 0.5;

    // Get the fragment's position in normalized device coordinates
    vec2 fragCoord = gl_FragCoord.xy / vec2(800, 800); // Assuming a resolution of 800x600
    // Map the fragment's position to the rectangle's coordinate system
    float x = left + (right - left) * fragCoord.x;
    float y = bottom + (top - bottom) * fragCoord.y;

    // Check if the fragment is inside the rectangle
    if (fragCoord.x >= left && fragCoord.x <= right && fragCoord.y >= bottom && fragCoord.y <= top) {
        // Set the fragment color to white if inside the rectangle
        FragColor = vec4(0.6, 1.0, 1.0, 1.0);
    } else {
        // Set the fragment color to black if outside the rectangle
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}