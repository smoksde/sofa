#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
layout (rgba32f, binding = 0) uniform image3D frame_buffer;

layout (std430, binding = 1) buffer position_data
{
    vec4 positions[];
};

layout (std430, binding = 2) buffer rotation_data
{
    vec4 rotations[];
};

layout (std430, binding = 3) buffer scale_data
{
    vec4 scales[];
};

uniform ivec3 grid_size;
uniform vec3 grid_origin;
uniform vec3 grid_scale;
uniform vec3 voxel_size;
uniform int num_objects;


bool isInsideCuboid(vec3 pos, vec3 cuboid_pos, vec3 cuboid_scale)
{
    vec3 half_scale = cuboid_scale * 0.5;
    
    return (pos.x >= cuboid_pos.x - half_scale.x && pos.x <= cuboid_pos.x + half_scale.x) &&
           (pos.y >= cuboid_pos.y - half_scale.y && pos.y <= cuboid_pos.y + half_scale.y) &&
           (pos.z >= cuboid_pos.z - half_scale.z && pos.z <= cuboid_pos.z + half_scale.z);
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
        mat3 rotation_matrix = eulerToRotationMatrix(rotations[i].xyz);

        vec3 local_pos = transpose(rotation_matrix) * (global_pos - positions[i].xyz);

        if (isInsideCuboid(local_pos, vec3(0.0, 0.0, 0.0), scales[i].xyz))
        {
            inside = true;
            ord = i+1;
            break;
        }
    }

    //float value = inside ? 1.0 : 0.0;
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    if (ord == 0)
    {
        color = vec4(0.0, 0.0, 0.0, 1.0);
    }
    if (ord == 1)
    {
        color = vec4(1.0, 0.0, 0.0, 1.0); 
    } 
    if (ord == 2)
    {
        color = vec4(0.0, 1.0, 0.0, 1.0);
    }  
    if (ord == 3)
    {
        color = vec4(0.0, 0.0, 1.0, 1.0);  
    }  
    if (ord == 4)
    {
        color = vec4(1.0, 1.0, 0.0, 1.0);  
    }  
    imageStore(frame_buffer, voxel_idx, color);
}