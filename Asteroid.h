#pragma once
#include "glew/glew.h"
#include "Triangle.h"
#include <vector>
#include <map>
#include <stdint.h>

using namespace std;
class Asteroid
{
public:
	Asteroid(void);
	~Asteroid(void);
	vector<GLfloat> getNormals();
	vector<GLfloat> getVertices();
	vector<GLuint> getElementArray();
private:
	map<int64_t, int> middlePointCache;
	vector<Point> asteroidPoints;
	vector<Triangle> asteroidTriangles;
	vector<GLfloat> asteroidGLVertices;
	vector<GLfloat> asteroidGLNormals;
	vector<GLuint> elements;
	void generateAsteroidBase();
	void createIcosahedronPoints();
	void createIcosahedronTriangles();
	void deformAndTransformBase();
	void deformAsteroid(vector<Point>* points);
	void copyBaseInformation();
	void transformToGL();
	int calculateMiddlePoint(int p1, int p2, vector<Point>* points);
	void refineIcoSphere();
	int affixVertex(Point point, Point derpPoint, vector<Point>* pointsVector);
};



