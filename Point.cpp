#include "Point.h"

Point::Point(GLfloat x, GLfloat y, GLfloat z)
{
	this->coords = glm::vec3(x,y,z);
}


Point::~Point(void)
{
}
