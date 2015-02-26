#include "Entity.h"
#include "Shape.h"
#include <string>
#include <math.h>
#include <glm\glm.hpp>           // All GLM data types
#include <glm\gtx\transform.hpp> // Matrix operations

using namespace glm;

Entity::Entity(Shape* sh, int t, glm::vec3 sc, glm::vec3 start)
{
	position = start;
	velocity=vec3(0,0,0);
	accel=vec3(0,0,0);
	scaleV = vec3(1,1,1);
	mouse = vec3(0,0,0);
	shape = sh;
	type = t;
	prevTime=0;
	scaleV = sc;
	waveProgression=0;
}

Entity::~Entity(void)
{
}

void Entity::Update()
{
	//cout << "update";
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int dt = currentTime - prevTime;	
	prevTime =currentTime;
	// floats on water and follows mouse
	if(type==0)
	{
		float dtScale = ((float)dt)/3300;
		waveProgression+=10*dtScale;

		//position.y = 0.1f*sin(waveProgression);
		if(!isXNear(mouse,0.1f))
		{
			if((mouse.x/400 - 1)>position.x)
			{
				accel.x = 0.001f;
				if(position.x>1.2f) position.x=-1.2f;
			}
			else if((mouse.x/400 - 1)<position.x)
			{
				accel.x = -0.001f;
				if(position.x<-1.2f) position.x=1.2f;
			}
			else
			{
				accel = -velocity;
				accel = vec3(accel.x/3,0,0);
			}
			
			if(isXNear(mouse,0.2f))
			{
				velocity = vec3(0,0,0);
				accel = vec3(0,0,0);
			}

			velocity*=0.7f;
			velocity += accel;
			float x = velocity.x;
			float y = velocity.y;
			float z = velocity.z;

			if(x*x+y*y+z*z>0.003f)
				velocity = normalize(velocity)*0.003f*dtScale;
			position+=velocity;
		}
		else
		{
			velocity = vec3(0,0,0);
		}
		position.y=0.02f*sin(waveProgression);
	}
	// case for the hexagons float up and avoid ship as much as possible.
	if(type==1)
	{
		position.y+=0.001f;
		if(position.y>1.05 || (isXNear(boat->position,0.15f) && isYNear(boat->position,0.15f)))
		{
			position.y=-1.05f;
			position.x=((float)rand()/RAND_MAX) * 2 - 1;
		}

		//position.y = 0.1f*sin(waveProgression);
		if(isXNear(boat->position,0.3f)&& isYNear(boat->position,0.3f))
		{
			if((boat->position.x)>position.x)
			{
				accel.x = -0.00002f;
				if(position.x>1.2f) position.x=-1.1f;
			}
			else if((boat->position.x)<position.x)
			{
				accel.x = 0.00002f;
				if(position.x<-1.2f) position.x=1.1f;
			}
			else
			{
				accel = normalize(-velocity);
				accel = vec3(accel.x/10,0,0);
			}

			velocity += accel;
			float x = velocity.x;
			float y = velocity.y;
			float z = velocity.z;

			if(x*x+y*y+z*z>0.0005f)
				velocity = normalize(velocity)*0.0005f;
			position+=velocity;
		}
		else
		{
			velocity = vec3(0,0,0);
		}
	}

	//accel = normalize(accel);
	//velocity/=2;
	//velocity+=accel;
	//position+=velocity;
	//accel=vec3(0,0,0);
}

//shrinks the total mag of a vec3 to 1
glm::vec3 Entity::normalize(glm::vec3 vector)
{
	float x = vector.x;
	float y = vector.y;
	float z = vector.z;

	return vector*vector/(x*x+y*y+z*z);
}

// draws the shape
void Entity::Draw()
{
	shape->Draw(position.x,position.y, scaleV.x,scaleV.y);
}

//checks to see if the 2D distance between this object and a point in space is less than 0.2f
bool Entity::isNear(glm::vec3 other)
{
	vec3 distanceVector = other;
	float x = distanceVector.x;
	float y = distanceVector.y;
	float z = distanceVector.z;
	float distance = glm::sqrt(x*x + y*y + z*z);

	if(distance < 0.2f)
		return true;

	return false;
}

//checks to see if the 2D x distance between this object and a point in space is less than dist
bool Entity::isXNear(glm::vec3 other, float dist)
{
	vec3 distanceVector = other-position;
	float x = abs(distanceVector.x);

	if(x < dist)
		return true;

	return false;
}

//checks to see if the 2D y distance between this object and a point in space is less than dist
bool Entity::isYNear(glm::vec3 other, float dist)
{
	vec3 distanceVector = other-position;
	float y = abs(distanceVector.y);

	if(y < dist)
		return true;

	return false;
}

// In progress to be used later
glm::vec3 Entity::Seek(glm::vec3 other)
{
	vec3 seek = vec3(0,0,0);
	if(other.x >1.0f || other.y>1.0f || other.x < -1.0f || other.y < -1.0f)
		return seek;
	else 
		return seek;
}

// In progress to be used later
glm::vec3 Entity::Flee(glm::vec3 other)
{
	return -Seek(other);
}