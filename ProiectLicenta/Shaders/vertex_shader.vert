#version 330 core

layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 vertex_normal;

out vec2 v_texCoord;

uniform mat4 model;
uniform mat4 vizualizare;
uniform mat4 proiectie;


void main(void)
{
	
	gl_Position =  proiectie * vizualizare * model * vec4(vertex_position_modelspace, 1);
	v_texCoord = texCoord;
} 
 