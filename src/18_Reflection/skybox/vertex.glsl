#version 440

layout(location = 0) in vec3 position;

out vec3 _tex_coords;

uniform mat4 view;
uniform mat4 projection;

void main() {
    _tex_coords = position;

    // the swizzling trick ensures that depth value is always 1 after the /w division
    vec4 pos = projection * view * vec4(position, 1.0f);
    gl_Position = pos.xyww;
}