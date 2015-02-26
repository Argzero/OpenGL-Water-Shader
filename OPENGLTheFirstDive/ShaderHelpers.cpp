#include "ShaderHelpers.h"

// Constructor and Destructor
ShaderHelpers::ShaderHelpers(void)
{
}
ShaderHelpers::~ShaderHelpers(void)
{
}

// The following three functions all load in a pair of shaders and store them by giving them to OpenGL
char* ShaderHelpers::loadTextFile(const char* file) // loads a text file
{
	ifstream inFile;

	inFile.open(file, ios::binary);

	if( inFile.is_open() )
	{
		inFile.seekg(0, ios::end);
		int length =(int)inFile.tellg();

		inFile.seekg(0, ios::beg);
		char* fileContents = new char[length+1];

		inFile.read(fileContents, length);
		fileContents[length]= 0;
		inFile.close();
		return fileContents;
	}
	else
		return 0;
}
GLuint ShaderHelpers::loadShader(const char* file, GLenum shaderType) // loads and compiles a shader
{
	const char* shader = loadTextFile(file);
	cout<< shader;
	if(shader == 0)
	{
		cout << "ERROR!!!";
		return 0;
	}
	
	GLuint index = glCreateShader(shaderType);
	glShaderSource(index, 1, &shader, 0);
	glCompileShader(index);
	
	GLint status;
	glGetShaderiv(index, GL_COMPILE_STATUS, &status );

	if(!(status==GL_TRUE))
	{
		/*GLint log;
		glGetShaderiv(GL_INFO_LOG_LENGTH, &log);
		char* logArray = new char[log];
		glGetProgramInfoLog(programObj, 1, 0, logArray);
		cout << logArray << endl;

		delete[] logArray;
		glDeleteProgram(programObj);*/
		return 0;
	}
	return index;
	delete[] shader;
}
GLuint ShaderHelpers::loadShaderProgram(const char* vertexFile, const char* fragmentFile) // links the shaders to the shader program object
{ 
	GLuint vertex = loadShader(vertexFile, GL_VERTEX_SHADER);

	GLuint fragment = loadShader(fragmentFile, GL_FRAGMENT_SHADER);

	if(vertex==0)
		return 0;

	if(fragment==0)
		return 0;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	GLint iv; 
	glGetProgramiv(program, GL_LINK_STATUS, &iv); 

	if(iv==GL_TRUE)
	{
		return program;
	}
	else
	{
		GLint log;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log);
		char* logArray = new char[log];
		glGetProgramInfoLog(program, 1, 0, logArray);
		cout << logArray << endl;

		delete[] logArray;
		glDeleteProgram(program);
		return 0;
	}
}

// the following six functions can be used to set shader variables
void ShaderHelpers::setShaderColor(GLuint index, const char* name, float r, float g, float b) // sets a shader's color to r,g,b
{
	GLint loc = glGetUniformLocation(index, "color");
	glProgramUniform4f(index, loc, r, g, b, 1.0);
}
void ShaderHelpers::setShaderPosition(GLuint index, const char* name, glm::vec2 vecToSend) // sets a shader's position variable wih a glm::vec2
{
	GLint varLoc = glGetUniformLocation(index, name);
	glProgramUniform2fv(index, varLoc, 1, &vecToSend[0]);
}
void ShaderHelpers::setShaderPosition2F(GLuint index, const char* name, float x, float y) // sets a shader's position variable wih a pair of floats
{
	GLint varLoc = glGetUniformLocation(index, name);
	glProgramUniform2f(index, varLoc, x, y);
}

void ShaderHelpers::setShaderMatrix(GLuint index, const char* name, mat4 worldMatrix)
{
		GLint varLoc = glGetUniformLocation(index, name);
		glProgramUniformMatrix4fv(index, varLoc, 1, GL_FALSE, &worldMatrix[0][0]);
}

void ShaderHelpers::setShaderVec2(GLuint index, const char* name, glm::vec2 vecToSend) // sets a shader's position variable wih a glm::vec2
{
	GLint varLoc = glGetUniformLocation(index, name);
	glProgramUniform2fv(index, varLoc, 1, &vecToSend[0]);
}

void ShaderHelpers::setShaderVec3(GLuint index, const char* name, glm::vec3 vecToSend) // sets a shader's position variable wih a glm::vec2
{
	GLint varLoc = glGetUniformLocation(index, name);
	glProgramUniform3fv(index, varLoc, 1, &vecToSend[0]);
}