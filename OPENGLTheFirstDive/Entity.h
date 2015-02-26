#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <glm\glm.hpp>           // All GLM data types
#include <glm\gtx\transform.hpp> // Matrix operations
#include "Shape.h"

using namespace glm;

// The brain for the object
class Entity
{
public:
	Entity(Shape* sh, int t, glm::vec3 sc, glm::vec3 start);
	~Entity(void);
	
	// functions for general updates and draws to the screen of the shape pointer
	void Update();
	void Draw();

	int type; // is it a ship or a bubble?

	//check distance between objects for simple circular collisions.
	bool isNear(glm::vec3 other);
	bool isXNear(glm::vec3 other, float dist);
	bool isYNear(glm::vec3 other, float dist);
	glm::vec3 normalize(glm::vec3 vector);
	
	// In progress to be used later
	glm::vec3 Seek(glm::vec3 other);
	glm::vec3 Flee(glm::vec3 other);
	
	// vars
	Shape* shape;
	vec3 position;
	vec3 offset;
	vec3 startPosition;
	vec3 velocity;
	vec3 accel;
	vec3 scaleV;
	vec3 rotationV;
	vec3 mouse;
	Entity* boat;
	int prevTime;
	float angle;
	float waveProgression;
};

