#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "ShaderHelpers.h"
#include "Shape.h"
#include "Entity.h"
#include <math.h>                /* cos */
#include <glm\glm.hpp>           // All GLM data types
#include <glm\gtx\transform.hpp> // Matrix operations
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <cstdlib>
#include <ctime>

// For music
#include <windows.h>
#include <MMsystem.h>

using namespace std;
using namespace glm;

#define PI 3.14159265f

// global shapes
Shape* fishTail;
Shape* fishHead;
Shape* oFloor;
Shape* water;
Shape* star;
Shape* hexagon;
Shape* box;
Shape* ship;
Entity* boat;
Entity* bubbles[17];
Entity* entities[18];

float bPos=1.0f;
GLuint vertexBufferObject;
GLuint vertexArrayObject;
int prevTime = 0;
const GLsizei vertices = 24;
const GLsizei data = 48;
GLuint programObj;

// ShaderHelpers object deals with all loading of shaders.
ShaderHelpers helper;
glm::vec2 position;
glm::vec2 start;

// coordinates for the star objects to be randomized at a later point 
float Ys[100];
float Xs[100];

void init();
void update();
void DrawFish(float x, float y, float xS, float yS);
void draw();
void mouseMove(int,int);
void mouseCallback(int,int,int,int);
void drawWater();
void drawShapes();

int main(int argc, char** argv)
{
	// Plays the bgm music
	PlaySound(TEXT("DDD.wav"),NULL, SND_LOOP | SND_ASYNC);
	
	srand((unsigned int)time(0));
	for(int i = 0; i<100; i++)
	{
		Ys[i] = (((float)rand())/RAND_MAX)*0.7f+0.3f;
		Xs[i] = ((float)rand())/RAND_MAX*2-1;
	}

	glewExperimental=GL_TRUE;
	glutInit(&argc,argv);
	glutInitWindowSize(800,600);
	glutInitContextVersion(4,3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Titor.");
	if(glewInit() != GLEW_OK) { return -1; }
	glutIdleFunc(update);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMove);
	glutDisplayFunc(draw);
	init();
	gluOrtho2D(-1,365,-1,1);
	glutMainLoop();

	return 0;
}

// detects position of mouse when called
void mouseCallback(int button, int state, int x, int y)
{
	cout << "Mouse moved to: " << x << " " << y << endl;
}

void init()
{
	glClearColor(0,0,0.3f,0.3f);

	// designates the program to be used
	programObj = helper.loadShaderProgram("Shaders/vertexShader.glsl","Shaders/fragmentShader.glsl");
	if(programObj != 0)
	{
		glUseProgram(programObj);
	}

	// designates coordinates for the following shapes
	GLfloat coords[data] = {0,-200.0f,-1.0f,-200.0f,-1.0f,0,-0.9f,0,-0.8f,0,-0.7f,0,-0.6f,0, -0.5f, 0, -0.4f,0,-0.3f,0,-0.2f,0,-0.1f,0,0,0,.1f,0,.2f,0,.3f,0,.4f,0,.5f,0,.6f,0,.7f,0,.8f,0,.9f,0,1.0f,0,1.0f,-200.0f};	
	GLfloat ft[6] = {0,0,.1f,.2f,.1f,-.2f};
	GLfloat s[48] = {0,0,-1.0f,0,-0.5f,0.5f,0,0,-0.5f,0.5f,0,1.0f,0,0,0,1.0f,0.5f,0.5f,0,0,0.5f,0.5f,1.0f,0,0,0,1.0f,0,0.5f,-0.5f,0,0,0.5f,-0.5f,0,-1.0f,0,0,0,-1.0f,-0.5f,-0.5f,0,0,-0.5f,-0.5f,-1.0f,0};
	GLfloat fh[12] = {0,0,-0.2f,0.2f,-0.2f,-0.2f,-0.4f,0,-0.2f,0.2f,-0.2f,-0.2f};
	GLfloat fr[18] = {-1.0f,-0.9f,-1.0f,-1.0f,1.0f,-0.9f,-1.0f,-1.0f,1.0f,-0.9f,1.0f,-1.0f,-1.0f,-0.9f,0,-0.8f,1.0f,-0.9f};
	GLfloat b[12] = {-0.02f,1.0f,0.02f,1.0f,-0.02f,-1.0f,0.02f,1.0f,0.02f,-1.0f,-0.02f,-1.0f};
	GLfloat sailboat[36] = {-1.0f,0,-0.5f,0,-0.5f,-0.35f,-0.5f,0,-0.5f,-0.35f,0.5f,0,-0.5f,-0.35f,0.5f,0,0.5f,-0.35f,0.5f,0,0.5f,-0.35f,1.0f,0,0,0,0,0.25f,0,0.5f,0,0.5f,-0.5f,0.5f,0,1.0f};
	GLfloat hex[36] = {-1.0f,0,0,0,-0.75f,0.75f,-0.75f,0.75f,0,0,0.75f,0.75f,0.75f,0.75f,0,0,1.0f,0,1.0f,0,0,0,0.75f,-0.75f,0.75f,-0.75f,0,0,-0.75f,-0.75f,-0.75f,-0.75f,0,0,-1,0};

	// creates the shapes with the designated above sets of coordinates
	ship = new Shape(sailboat, 18, programObj, false, true, false, &helper);
	fishTail = new Shape(ft,3,programObj,false,false,false,&helper);
	fishHead = new Shape(fh,6,programObj,false,false,false,&helper);
	oFloor = new Shape(fr,9,programObj,false,false,false,&helper);
	star = new Shape(s,24,programObj,false,true,false,&helper);
	water = new Shape(coords,23,programObj,true,false,false,&helper);
	box = new Shape(b,6,programObj,false,false,true,&helper);
	hexagon = new Shape(hex, 18, programObj, false,true,false,&helper);

	// generating all entities initially
	boat = new Entity(ship,0,vec3(0.2f,0.2f,0.2f), vec3(0,0,0));

	entities[0] = boat;
	for(int i = 0; i < 17; i++)
	{
		bubbles[i] = new Entity(hexagon,1,vec3(0.02f,0.02f,1.0f), vec3(((float)rand()/RAND_MAX) * 2 - 1,-((float)rand()/RAND_MAX)-1,0));
		entities[i+1] = bubbles[i];
		bubbles[i]->boat = boat;
	}
}

// draws the beautiful water
void drawWater()
{
	//cout << "update";
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int dt = currentTime - prevTime;

	int varLoc = glGetUniformLocation(programObj, "time");
	helper.setShaderColor(programObj, "color", 0,0.35f,0.5f);//cout << "update";
	water->Draw(0,0,1.0f,1.0f);

	helper.setShaderColor(programObj, "color", 0,0.35f,0.8f);//cout << "update";
	water->Draw(0,-0.20f,1.0f,1.0f);
	water->Draw(0,-0.40f,1.0f,1.0f);
	water->Draw(0,-0.60f,1.0f,1.0f);
	water->Draw(0,-0.80f,1.0f,1.0f);
	water->Draw(0,-1.0f,1.0f,1.0f);
}

// GL update function
void update()
{
	for(int i = 0; i< 9; i++)
	{
		entities[i]->Update();
	}

	glutPostRedisplay();
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	prevTime = currentTime;	// Save for next frame
}

// clamps all colors passed in to usable value
float clampColor(int val)
{
	if(val >250)
		return 250.0f;
	else 
		return (float)val;
}

// responds to mouse motion, no longer effective
void mouseMove(int x, int y)
{
	cout << "Mouse moved to: " << x << " " << y << endl;
	vec3 mousePos = vec3(x,y,0);
	boat->mouse = mousePos;
	//helper.setShaderColor(programObj, "color", clampColor(x),clampColor(y),1);
}

// draws all the things and clears the gl window
void draw()
{
	//cout << "draw";
	glClear(GL_COLOR_BUFFER_BIT);
	
	//water 
	drawWater();

	//fish and floor
	drawShapes();

	for(int i = 0; i< 18; i++)
	{
		entities[i]->shape->Draw(entities[i]->position.x,entities[i]->position.y,entities[i]->scaleV.x,entities[i]->scaleV.y);
	}

	glFlush();
}

// draws !water :P
void drawShapes()
{
	// gets the time and the change in time, for fun
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int dt = currentTime - prevTime;
	
	// randomizes the star-hiding boxes x movement to avoid viewers noticing an obvious pattern of motion for the box 
	bPos-=0.0001f*(((float)rand())/RAND_MAX);
	if(bPos < -1.1f)
		bPos=1.1f;

	// draws the starfield
	for(int i = 0; i < 100; i++)
	{
		star->Draw(Xs[i],Ys[i],0.0025f,0.0025f);
	}

	
	// draws boxes to cause star flickering effect
	for(int i = 0; i < 20; i++)
	{
		box->Draw((bPos+i*0.05f)-1.0f,1.3f,0.02f,1.0f);
	}


	// unique big fish
	DrawFish(-0.6f,-0.5f,0.35f,0.3f);
	DrawFish(-0.4f,-0.2f,0.2f,0.25f);
	
	// drawing lots of fish
	for(float i = 0; i < 10.0f; i++)
	{
		float j = i;
		int iteration=0;
		while(j>3.0f)
		{
			j-=3.0f;
			iteration++;
		}

		if(iteration<1.0f)
			iteration=1;

		DrawFish(j/10.0f + iteration*0.05f*j -0.35f,j/10.0f - 0.5f,0.05f,0.05f);
	}

	// moar fish
	for(float i = 0; i < 10.0f; i++)
	{
		float j = i;
		int iteration=0;
		while(j>3.0f)
		{
			j-=3.0f;
			iteration++;
		}

		if(iteration<1.0f)
			iteration=1;

		DrawFish(j/10.0f + iteration*0.05f*j +0.15f,j/10.0f - 0.5f,0.05f,0.05f);
	}

	// EVEN MOAR FISH
	for(float i = 0; i < 10.0f; i++)
	{
		float j = i;
		int iteration=0;
		while(j>3.0f)
		{
			j-=3.0f;
			iteration++;
		}

		if(iteration<1.0f)
			iteration=1;

		DrawFish((j/10.0f + iteration*0.05f*j)/3.0f-0.85f,j/10.0f - 0.5f,0.05f,0.05f);
	}

	// FISHY FISHY FISH, MUCH MORE FISH
	for(float i = 0; i < 10.0f; i++)
	{
		float j = i;
		int iteration=0;
		while(j>3.0f)
		{
			j-=3.0f;
			iteration++;
		}

		if(iteration<1.0f)
			iteration=1;

		DrawFish(j/10.0f + iteration*0.05f*j,j/10.0f - 0.5f,0.05f,0.05f);
	}

	// seafloor
	oFloor->Draw(0,0,1.0f,1.0f);
}

// draws a fish at offset: x,y, scale: xS,yS
void DrawFish(float x, float y, float xS, float yS)
{
	GLint fish = glGetUniformLocation(programObj, "fish");
	glProgramUniform1i(programObj, fish, true);

	GLint fishOff = glGetUniformLocation(programObj, "fishOff");
	glProgramUniform1f(programObj, fishOff, x);

	fishHead->Draw(x,y,xS,yS);
	fishTail->Draw(x,y,xS,yS);
	glProgramUniform1i(programObj, fish, false);
	glProgramUniform1f(programObj, fishOff, 0);
}