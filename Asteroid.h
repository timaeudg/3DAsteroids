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
	Asteroid();
	Asteroid(glm::vec3 startingPosition, int elementOffset, GLfloat radius);
	~Asteroid(void);

	glm::mat4 getTransformMatrix(float bounds);
	glm::vec3 getPosition();
	GLfloat getRadius();
	vector<GLfloat> getNormals();
	vector<GLfloat> getVertices();
	vector<GLuint> getElementArray();
	vector<GLfloat> getColors();

private:
	map<int64_t, int> middlePointCache;
	vector<Point> asteroidPoints;
	vector<Triangle> asteroidTriangles;
	vector<GLfloat> asteroidGLVertices;
	vector<GLfloat> asteroidGLNormals;
	vector<GLuint> elements;
	vector<GLfloat> colors;

	int elementOffset;

	glm::vec3 position;
	glm::mat4 rotation;
	glm::mat4 translation;

	glm::vec3 velocityVector;
	glm::vec3 rotationRates;

	GLfloat radius;
	GLfloat deformedResultantRadius;

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
	void calculateDeformedResultantRadius();
};



