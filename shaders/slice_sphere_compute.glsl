#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
layout (rgba32f, binding = 0) uniform image3D frame_buffer;

layout(std430, binding = 1) buffer object_data {
    vec4 objects[]; // object data (center.x, center.y, center.z, radius)
};

uniform ivec3 grid_size;
uniform vec3 grid_origin;
uniform vec3 voxel_size;

bool isInsideSphere(vec3 pos, vec4 sphere) {
    vec3 center = sphere.xyz;
    float radius = sphere.w;
    return distance(pos, center) < radius;
}

void main() {
    ivec3 voxel_idx = ivec3(gl_GlobalInvocationID);
    if (voxel_idx.x >= grid_size.x || voxel_idx.y >= grid_size.y || voxel_idx.z >= grid_size.z) return;

    vec3 voxel_pos = grid_origin + vec3(voxel_idx) * voxel_size;

    bool inside = false;
    for (int i = 0; i < objects.length(); i++) {
        if (isInsideSphere(voxel_pos, objects[i])) {
            inside = true;
            break;
        }
    }

    float value = inside ? 1.0 : 0.0;
    imageStore(frame_buffer, voxel_idx, vec4(value, value, value, 1.0));
}