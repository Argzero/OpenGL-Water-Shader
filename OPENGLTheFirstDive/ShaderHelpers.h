#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <glm\glm.hpp>           // All GLM data types
#include <glm\gtx\transform.hpp> // Matrix operations

using namespace std;
using namespace glm;
class ShaderHelpers
{
public:
	// Creates a ShaderHelpers
	ShaderHelpers(void);

	// Destructs a ShaderHelpers
	~ShaderHelpers(void);

	// loads a set of shader glsl files and stores them
	char* loadTextFile(const char* file);
	GLuint loadShader(const char* file, GLenum shaderType);
	GLuint loadShaderProgram(const char* vertexFile, const char* fragmentFile);

	// sets attributes in the shaders
	void setShaderColor(GLuint index, const char* name, float r, float g, float b);	
	void setShaderPosition(GLuint index, const char* name, glm::vec2 vecToSend);
	void setShaderPosition2F(GLuint index, const char* name, float x, float y);

	void setShaderVec2(GLuint index, const char* name, glm::vec2 vecToSend);
	void setShaderMatrix(GLuint index, const char* name, mat4 worldMatrix);
	void setShaderVec3(GLuint index, const char* name, glm::vec3 vecToSend); // sets a shader's position variable wih a glm::vec3
};

