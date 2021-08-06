#version 330 core

layout(location=0) in vec4 in_Position;
layout(location=1) in vec2 coord_tex;

out vec4 gl_Position;
out vec2 texture_coord;
uniform mat4 mvp;

void main(void)
{
	gl_Position = mvp * in_Position;
	texture_coord = vec2(coord_tex.x, 1 - coord_tex.y);
}
