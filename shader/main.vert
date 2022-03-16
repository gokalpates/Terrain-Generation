#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float height;

void main()
{
	height = position.y;
	gl_Position = projection * view * model * vec4(position, 1.0);
}