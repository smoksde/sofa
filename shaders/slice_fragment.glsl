#version 430 core

in vec3 texCoord;  // UV coordinates from the vertex shader
uniform sampler3D voxel_texture;
uniform float slice_z;  // The specific Z-slice to visualize

out vec4 FragColor;

void main() {
    vec3 sample_pos = vec3(texCoord.xy, slice_z);  // Fixed Z for the slice, XY from UV
    FragColor = texture(voxel_texture, sample_pos);  // Sample from the 3D texture
}
