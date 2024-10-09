#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
layout (rgba32f, binding = 0) uniform image3D frame_buffer;

layout (std430, binding = 1) buffer inverse_model_data
{
    mat4 inverse_models[];
};

uniform ivec3 grid_size;
uniform vec3 grid_origin;
uniform vec3 grid_scale;
uniform vec3 voxel_size;
uniform int num_objects;

bool isInsideUnitCuboid(vec3 pos)
{
    return pos.x >= -0.5 && pos.x <= 0.5 && pos.y >= -0.5 && pos.y <= 0.5 && pos.z >= -0.5 && pos.z <= 0.5;
}

mat3 eulerToRotationMatrix(vec3 euler)
{
    mat3 r_x = mat3(1.0, 0.0, 0.0,
                    0.0, cos(euler.x), -sin(euler.x),
                    0.0, sin(euler.x), cos(euler.x));
    mat3 r_y = mat3(cos(euler.y), 0.0, sin(euler.y),
                    0.0, 1.0, 0.0,
                    -sin(euler.y), 0.0, cos(euler.y));
    mat3 r_z = mat3(cos(euler.z), -sin(euler.z), 0.0,
                    sin(euler.z), cos(euler.z), 0.0,
                    0.0, 0.0, 1.0);

    return r_z * r_y * r_x;
}

void main()
{
    ivec3 voxel_idx = ivec3(gl_GlobalInvocationID);
    vec3 global_pos = grid_origin + (((vec3(voxel_idx) / vec3(grid_size)) - 0.5) * grid_scale);

    bool inside = false;
    int ord = 0;

    for (int i = 0; i < num_objects; i++)
    {
        vec4 local_pos_4 = inverse_models[i] * vec4(global_pos.x, global_pos.y, global_pos.z, 1.0);
        vec3 local_pos = vec3(local_pos_4.x, local_pos_4.y, local_pos_4.z);

        if (isInsideUnitCuboid(local_pos))
        {
            inside = true;
            ord += 1;
        }
    }

    //float value = inside ? 1.0 : 0.0;
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    if (ord == 0)
    {
        color = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else if (ord == 1)
    {
        color = vec4(1.0, 0.0, 0.0, 1.0); 
    }
    else if (ord == 2)
    {
        color = vec4(0.0, 1.0, 0.0, 1.0); 
    }  
    else
    {
        color = vec4(0.0, 0.0, 1.0, 1.0);
    }
    imageStore(frame_buffer, voxel_idx, color);
}