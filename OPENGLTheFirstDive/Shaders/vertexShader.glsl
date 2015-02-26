#version 330
#define M_PI 3.1415926535897932384626433832795
#define FPI M_PI/4

layout (location = 0) in vec3 position;
uniform mat4 worldMatrix;
uniform float time;
uniform bool water = false;
uniform bool fish = false;
uniform float fishOff;
out float yPos;

void main()
{
	float yOff = 0.0f;
	float xOff = 0.0f;
	yPos = -1.0f;
	if(water)
	{
		yPos = (position.y + yOff);
		yOff = 0.05*sin(position.x*12+time*FPI);
		if((position.x != 1.0 && position.x != -1.0))
			xOff = 0.025*cos(position.x*12+time*FPI);
	}
	else if(fish)
	{
		float off = fishOff;
		if(off==0)
			off = 1.0f;
		yOff = 0.5f*4.0f*(0.5f)*off*sin(off+time*FPI);
		yPos = -1.0f;
	}
	
	gl_Position = worldMatrix * vec4((position.x - xOff),(position.y + yOff), position.z, 1);	
}