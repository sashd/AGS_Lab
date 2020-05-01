#version 330 core

uniform vec4 offset;
in vec2 vPosition;

out vec2 position;

void main ()
{
	gl_Position = vec4 (vPosition + vec2(offset), 0, 1);
	position = vPosition;
}
