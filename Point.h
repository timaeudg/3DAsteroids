#pragma once
#include "glew/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <math.h>
#include <ctime>
using namespace std;
class Point
{
public:
	glm::vec3 coords;
	Point(GLfloat x, GLfloat y, GLfloat z){
		this->coords = glm::vec3(x, y, z);
	}

	Point();
	~Point(void);

	bool moveInRandomDirection(GLfloat maxDistanceToMove){
		GLfloat maxDimensionDistance = pow(maxDistanceToMove, 2)/3.0;
		
		GLfloat xToMove = -maxDimensionDistance + (float)rand()/((float)RAND_MAX/(maxDimensionDistance-(-maxDimensionDistance)));
		GLfloat yToMove = -maxDimensionDistance + (float)rand()/((float)RAND_MAX/(maxDimensionDistance-(-maxDimensionDistance)));
		GLfloat zToMove = -maxDimensionDistance + (float)rand()/((float)RAND_MAX/(maxDimensionDistance-(-maxDimensionDistance)));

		this->coords.r += xToMove;
		this->coords.g += yToMove;
		this->coords.b += zToMove;
		return true;
	}
};



