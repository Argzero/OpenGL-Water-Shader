#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <glm\glm.hpp>           // All GLM data types
#include <glm\gtx\transform.hpp> // Matrix operations
#include "ShaderHelpers.h"
#define _USE_MATH_DEFINES // for C++
#include <cmath>

class Shape
{
public:
	// What is a shape constructor? :P
	Shape(GLfloat* coords, GLsizei dataSize, GLuint program, bool isWater, bool isStar, bool isBox, ShaderHelpers* help);
	
	// What is a shape destructor? :P
	~Shape(void);

	// GL's required variables
	GLuint vbo;
	GLuint vao;

	// Program containing the shaders
	GLuint programObj;
	
	// shape attributes
	int vertexCount;
	int dataCount;
	
	// Shader variables
	GLint matrix;
	bool water;
	bool star;
	bool box;

	// previous time
	float pTime;

	// accesses the global ShaderHelpers object
	ShaderHelpers* helper;

	// Draws the shape
	void Draw(float x, float y, float xScale, float yScale);
};

