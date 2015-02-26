#include "Shape.h"
#define _USE_MATH_DEFINES // for C++
#include <cmath>

#define _USE_MATH_DEFINES // for C
#include <math.h>

using namespace glm;

// What is a shape? :P
Shape::Shape(GLfloat* coords, GLsizei dataSize, GLuint program, bool isWater, bool isStar, bool isBox, ShaderHelpers* help)
{
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	vertexCount = dataSize;
	programObj=program;
	dataCount=2*vertexCount;
	helper = help;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*dataCount, coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	pTime = 0;
	matrix = glGetUniformLocation(program, "worldMatrix");

	// cases for types, guess I should've used an enum eh?
	water = isWater;
	star = isStar;
	box = isBox;
}

Shape::~Shape(void)
{
	glDeleteVertexArrays(1,&vao);
	glDeleteBuffers(1,&vbo);
}

void Shape::Draw(float x, float y, float xScale, float yScale)
{
	// gets time and sets dt for fun
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int dt = currentTime - (int)pTime;
	
	// sets the vertex shader's offset and scale values
	vec3 offsetV = vec3(x,y,0);
	vec3 scaleV = vec3(xScale, yScale, 1.0f);
	vec3 rotationV = vec3(1,0,0);
	float angle = 0;

	mat4 worldMatrix = translate(offsetV) * scale(scaleV) * rotate(angle,rotationV);
	
	//helper->setShaderVec3(programObj, "position", vec3(1,0,0.5f));
	helper->setShaderMatrix(programObj, "worldMatrix", worldMatrix);
	glBindVertexArray(vao);

	// sets the vertex shader's water boolean value
	GLint isWater = glGetUniformLocation(programObj, "water");
	glProgramUniform1i(programObj, isWater, water);

	if(water) // case for water
	{		
		int varLoc = glGetUniformLocation(programObj, "time");
		glProgramUniform1f(programObj,varLoc, currentTime*0.001f);
		
		
		glDrawArrays(GL_TRIANGLE_FAN,0,vertexCount);
	}
	else if(!star) // case if a fish or box or anything else which is neither a star nor water
	{
		helper->setShaderColor(programObj, "color", 0,0.5f,0.5f);

		if(box)
			helper->setShaderColor(programObj, "color", 0,0,0.3f);

		glDrawArrays(GL_TRIANGLES,0,vertexCount);
	}
	else // case for stars
	{
		helper->setShaderColor(programObj, "color", 1.0f,1.0f,1.0f); 
		glDrawArrays(GL_TRIANGLES,0,vertexCount);
	}

	// updates the current time
	pTime=(float)currentTime;
}
