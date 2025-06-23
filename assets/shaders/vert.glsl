#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 1) in int a_block_type;

uniform mat4 mvp;
uniform vec3 world_pos;

void main()
{
        gl_Position = mvp * vec4(world_pos + a_position, 1.0f);
}
