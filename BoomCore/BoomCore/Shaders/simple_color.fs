#version 330 core
int vec2 Texcoord;
int vec3 Normal;
int vec3 Color;

out vec4 color;

void main()
{
	color = vec4(Color,1.0f);
}