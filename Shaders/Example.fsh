#version 330 core

uniform vec4 color1;
uniform vec4 color2;

out vec4 FragColor;
in vec2 position;

void main()
{
	if(position.x < position.y)
	{
		FragColor = color1;
	}
	else
	{
		FragColor = color2;
	}
}
