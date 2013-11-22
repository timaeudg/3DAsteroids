#pragma once
#include "glew/glew.h"
using namespace std;
class Wall
{
public:
	static const GLfloat Wall::points[8*3*3+4*3];
	static const GLuint Wall::elements[30+6];
	static const GLfloat Wall::colors[8*3*3+4*3];
	static const GLfloat Wall::normals[8*3*3+4*3];
	Wall(void);
	~Wall(void);
};



