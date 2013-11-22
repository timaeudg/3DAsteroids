#ifndef __MAZEMODEL
#define __MAZEMODEL

#include <vector>
#include "Maze.h"
using namespace std; //makes using vectors easy

class MazeModel
{
public:
	size_t height;
	size_t width;
	MazeModel()
	{
	}
	
	MazeModel(Maze const & maze)
	{
		height = maze.getHeight();
		width = maze.getWidth();
		size_t h = maze.getHeight();
		size_t w = maze.getWidth();
		float unit = this->getUnitSize();
		
		size_t maxMazeWalls = w*(h+1) + h*(w+1);
		size_t maxElements = maxMazeWalls*2;
		
		for(size_t y=0; y<h+1; y++)
		{
			for(size_t x=0; x<w+1; x++)
			{
				positions.push_back(x*unit);
				positions.push_back(y*unit);
				positions.push_back(0);
			}
		}
		
		//bottom lines
		for(size_t y=0; y<h; y++)
		{
			for(size_t x=0; x<w; x++)
			{
				bool blocked = maze.bottomBlocked(x, y);
				if(blocked)
				{
					elements.push_back(y*(w+1)+x);
					elements.push_back(y*(w+1)+x+1);
				}
			}
		}
		
		//top lines, top row
		for(size_t x=0; x<w; x++)
		{
			bool blocked = maze.topBlocked(x, h-1);
			if(blocked)
			{
				elements.push_back(h*(w+1)+x);
				elements.push_back(h*(w+1)+x+1);
			}
		}
		
		//left lines
		for(size_t y=0; y<h; y++)
		{
			for(size_t x=0; x<w; x++)
			{
				bool blocked = maze.leftBlocked(x, y);
				if(blocked)
				{
					elements.push_back(y*(w+1)+x);
					elements.push_back((y+1)*(w+1)+x);
				}
			}
		}
		
		//right col
		for(size_t y=0; y<h; y++)
		{
			bool blocked = maze.rightBlocked(w-1, y);
			if(blocked)
			{
				elements.push_back( y   *(w+1)+h);
				elements.push_back((y+1)*(w+1)+h);
			}
		}
		
		//This is just to make the GPU buffer the max size it could be
		while(elements.size() < maxElements)
			elements.push_back(0);
	}
	
	GLfloat const * getPosition() const
	{ return &positions[0]; }

	GLfloat const * getPositions() const {
		return positions.data();
	}
	
	GLuint const * getElements() const
	{ return &elements[0]; }
	
	size_t getElementCount() const
	{ return elements.size(); }
	
	size_t getElementBytes() const
	{ return elements.size()*sizeof(GLuint); }
	
	size_t getVertexCount() const
	{ return positions.size()/3; }
	
	size_t getPositionBytes() const
	{ return positions.size()*sizeof(GLfloat); }
	
	float getUnitSize()
	{ return 1.0f; }
	
private:
	vector<GLfloat> positions;
	vector<GLuint> elements;
};

#endif
