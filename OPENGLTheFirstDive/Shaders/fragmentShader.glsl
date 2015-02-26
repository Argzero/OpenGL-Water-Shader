#version 330

uniform vec4 color = vec4(1, 1, 0, 1);
in float yPos;

void main()
{
	gl_FragColor = color*(-yPos);
}