#version 330 core

in vec2 v_texCoord;

out vec3 color;

uniform sampler2D myTextureSampler;

void main(void)
{
    color = texture(myTextureSampler, v_texCoord).rgb;
}
 