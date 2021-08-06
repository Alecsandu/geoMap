#version 330 core

in vec2 texture_coord;
out vec4 out_color;

uniform sampler2D myTexture;

void main(void)
{
	out_color = mix(texture(myTexture, texture_coord), vec4(1.0, 0.0, 0.0, 1.0), 0);
}
