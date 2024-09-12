#version 330 core

#define M_PI 3.1415926535897932384626433832795
#define RESOLUTION_N 1000

in vec2 MyCoord;

out vec4 FragColor;

uniform int N;
uniform float angle[RESOLUTION_N]; // Rotation angle in radians
uniform float offsetX[RESOLUTION_N]; // X offset
uniform float offsetY[RESOLUTION_N]; // Y offset

void main()
{
    bool touched = false;

    vec2 pivot = vec2(0.0, 0.25);

    for (int i = 0; i < N; i++)
    {
        vec2 coords = MyCoord.xy;
        coords = vec2(coords.x + offsetX[i], coords.y + offsetY[i]);
        coords -= pivot;
        mat2 rotation_matrix = mat2(cos(angle[i]), -sin(angle[i]), sin(angle[i]), cos(angle[i]));
        coords = coords * rotation_matrix;
        coords += pivot;
        vec2 final_coords = coords;
        if (final_coords.x < 0.0 && 0.25 < final_coords.y && 0.75 > final_coords.y)
        {
            // FragColor = vec4(1.0, 1.0, 1.0, 0.0);
            continue;
        }

        if (0.0 < final_coords.x && final_coords.x < 0.5 && 0.25 < final_coords.y && 0.75 > final_coords.y)
        {
            // FragColor = vec4(1.0, 1.0, 1.0, 0.0);
            continue;
        }

        if (0.0 < final_coords.x && final_coords.x < 0.5 && 0.25 > final_coords.y)
        {
            // FragColor = vec4(1.0, 1.0, 1.0, 0.0);
            continue;
        }

        touched = true;
        break;
    }


    if (touched)
    {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    return;
}
