#version 410 core

layout(location = 0) in vec3 position;

uniform vec3 color;

out vec4 v_Color;

void main()
{
	gl_Position = vec4(position, 1.0);
	v_Color = vec4(color, 1.0f);
}
