#version 330 core

layout (location = 0) in vec3 a_normal;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in uint a_packed_coord_type;

out vec2 v_uv;

uniform mat4 mvp;
uniform vec3 world_pos;

void main()
{
        uint a_x = (a_packed_coord_type >> 24u) & 0xFFu;
        uint a_y = (a_packed_coord_type >> 16u) & 0xFFu;
        uint a_z = (a_packed_coord_type >> 8u) & 0xFFu;

        uint a_type = a_packed_coord_type & 0xFFu;

        vec3 a_position = vec3(a_x, a_y, a_z);

        v_uv = a_uv;
        gl_Position = mvp * vec4(world_pos + a_position, 1.0f);
}
