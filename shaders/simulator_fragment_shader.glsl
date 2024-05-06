#version 330 core

#define M_PI 3.1415926535897932384626433832795

in vec2 MyCoord;

out vec4 FragColor;

uniform float angle; // Rotation angle in radians
uniform float offsetX; // X offset
uniform float offsetY; // Y offset

void main()
{
    vec2 coords = MyCoord.xy;

    coords = vec2(coords.x + offsetX, coords.y + offsetY);

    // Rotate coords
    float radians = -angle * M_PI / 180.0;
    mat2 rotation_matrix = mat2(cos(radians), -sin(radians), sin(radians), cos(radians));
    coords = coords * rotation_matrix;

    vec2 final_coords = coords;

    if (final_coords.x < 0.0 && 0.0 < final_coords.y && 0.5 > final_coords.y)
    {
        FragColor = vec4(1.0, 1.0, 1.0, 0.0);
        return;
    }

    if (0.0 < final_coords.x && final_coords.x < 0.5 && 0.0 < final_coords.y && 0.5 > final_coords.y)
    {
        FragColor = vec4(1.0, 1.0, 1.0, 0.0);
        return;
    }

    if (0.0 < final_coords.x && final_coords.x < 0.5 && 0.0 > final_coords.y)
    {
        FragColor = vec4(1.0, 1.0, 1.0, 0.0);
        return;
    }
        
    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    return;
    


    // Define the center of the square
    // vec2 center = vec2(0.5, 0.5);
    
    // Apply the offsets
    // vec2 shiftedCoord = MyCoord + vec2(offsetX, offsetY);
    
    // Calculate the coordinates relative to the center
    // vec2 relCoord = shiftedCoord - center;
    
    // Perform the rotation using the provided angle
    // float rotatedX = relCoord.x * cos(angle) - relCoord.y * sin(angle);
    // float rotatedY = relCoord.x * sin(angle) + relCoord.y * cos(angle);
    
    // Apply rotation and render if inside the square
    // if (abs(rotatedX) < 0.5 && abs(rotatedY) < 0.5) {
    //    FragColor = vec4(1.0, 1.0, 1.0, 1.0); // Render the square as white
    //} else {
    //    FragColor = vec4(0.5, 0.5, 1.0, 1.0);
    //}

    // FragColor = vec4(0.6, 0.8, 0.8, 1.0);
}
