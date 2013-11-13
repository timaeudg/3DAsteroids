#include "Asteroid.h"
#define REFINEMENT_LEVEL 2
#define BASE_RADIUS 1.9021f
#define MAX_VELOCITY 0.03
#define MAX_ROTATION 5.0

Asteroid::Asteroid(){
	Asteroid(glm::vec3(0), 0, 1.9021);
}

Asteroid::Asteroid(glm::vec3 startingPosition, int elementOffset, int radius)
{
	this->radius = radius;
	this->position = startingPosition;
	this->elementOffset = elementOffset;
	generateAsteroidBase();
	deformAndTransformBase();
	calculateDeformedResultantRadius();

	glm::vec3 translateVector = this->position;
	this->translation = glm::translate(glm::mat4(1), translateVector);

	GLfloat xPos = -MAX_VELOCITY + (float)rand()/((float)RAND_MAX/(MAX_VELOCITY-(-MAX_VELOCITY)));
	GLfloat yPos = -MAX_VELOCITY + (float)rand()/((float)RAND_MAX/(MAX_VELOCITY-(-MAX_VELOCITY)));
	GLfloat zPos = -MAX_VELOCITY + (float)rand()/((float)RAND_MAX/(MAX_VELOCITY-(-MAX_VELOCITY)));
	velocityVector = glm::vec3(xPos, yPos, zPos);

	GLfloat xRot = -MAX_ROTATION + (float)rand()/((float)RAND_MAX/(MAX_ROTATION-(-MAX_ROTATION)));
	GLfloat yRot = -MAX_ROTATION + (float)rand()/((float)RAND_MAX/(MAX_ROTATION-(-MAX_ROTATION)));
	GLfloat zRot = -MAX_ROTATION + (float)rand()/((float)RAND_MAX/(MAX_ROTATION-(-MAX_ROTATION)));
	this->rotationRates = glm::vec3(xRot, yRot, zRot);
	this->rotation = glm::mat4(1);
}

Asteroid::~Asteroid(void)
{
}

vector<GLfloat> Asteroid::getNormals(){
	return this->asteroidGLNormals;
}

vector<GLfloat> Asteroid::getVertices(){
	return this->asteroidGLVertices;
}

vector<GLuint> Asteroid::getElementArray(){
	return elements;
}

vector<GLfloat> Asteroid::getColors(){
	return colors;
}

glm::vec3 Asteroid::getPosition(){
	return this->position;
}

glm::mat4 Asteroid::getTransformMatrix(float bounds){
	this->position += velocityVector;
	glm::mat4 T = glm::translate(this->translation, velocityVector);
	this->translation = T;
	if(this->position.x + radius >= bounds || this->position.x - radius <= -bounds){
		velocityVector = glm::vec3(-velocityVector.x, velocityVector.y, velocityVector.z);
		rotationRates = glm::vec3(rotationRates.x, -rotationRates.y, -rotationRates.z);
	}

	if(this->position.y + radius >= bounds || this->position.y - radius <= -bounds){
		velocityVector = glm::vec3(velocityVector.x, -velocityVector.y, velocityVector.z);
		rotationRates = glm::vec3(-rotationRates.x, rotationRates.y, -rotationRates.z);
	}

	if(this->position.z + radius>= bounds || this->position.z - radius<= -bounds){
		velocityVector = glm::vec3(velocityVector.x, velocityVector.y, -velocityVector.z);
		rotationRates = glm::vec3(-rotationRates.x, -rotationRates.y, rotationRates.z);
	}

	glm::mat4 R = glm::rotate(this->rotation, rotationRates.x, glm::vec3(1,0,0));
	R = glm::rotate(R, rotationRates.y, glm::vec3(0,1,0));
	R = glm::rotate(R, rotationRates.z, glm::vec3(0,0,1));
	this->rotation = R;

	glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(radius / deformedResultantRadius, radius / deformedResultantRadius, radius / deformedResultantRadius));

	return T*R*S;
}

void Asteroid::generateAsteroidBase(){
	if(this->asteroidPoints.size() == 0){
		//We haven't made the base of the asteroid, so we need to
		createIcosahedronPoints();
		createIcosahedronTriangles();
		//Now we have the icosahedron, we should make it a bit better though
		
		for(int i = 0; i < REFINEMENT_LEVEL; i++){
			deformAsteroid(&this->asteroidPoints);
			refineIcoSphere();
		}
	}
}

void Asteroid::deformAndTransformBase(){
	transformToGL();

	vector<GLuint> elements = vector<GLuint>();

	for(int i = 0; i < this->asteroidTriangles.size()*3; i++){
		elements.push_back(i+this->elementOffset);
	}
	this->elements = elements;
		
	for(int i = 0; i < this->asteroidGLVertices.size(); i++){
		this->colors.push_back(1.0f);
		this->colors.push_back(1.0f);
		this->colors.push_back(1.0f);
	}
}

void Asteroid::transformToGL(){
	for(int i = 0; i < this->asteroidTriangles.size(); i++){
		Triangle* tri = &this->asteroidTriangles[i];
		tri->recalculateNormal(this->asteroidPoints[tri->p1Index], this->asteroidPoints[tri->p2Index], this->asteroidPoints[tri->p3Index]);

		this->asteroidGLVertices.push_back(this->asteroidPoints[tri->p1Index].coords.r);
		this->asteroidGLVertices.push_back(this->asteroidPoints[tri->p1Index].coords.g);
		this->asteroidGLVertices.push_back(this->asteroidPoints[tri->p1Index].coords.b);
										   
		this->asteroidGLVertices.push_back(this->asteroidPoints[tri->p2Index].coords.r);
		this->asteroidGLVertices.push_back(this->asteroidPoints[tri->p2Index].coords.g);
		this->asteroidGLVertices.push_back(this->asteroidPoints[tri->p2Index].coords.b);
										  
		this->asteroidGLVertices.push_back(this->asteroidPoints[tri->p3Index].coords.r);
		this->asteroidGLVertices.push_back(this->asteroidPoints[tri->p3Index].coords.g);
		this->asteroidGLVertices.push_back(this->asteroidPoints[tri->p3Index].coords.b);

		for(int k = 0; k < 3; k++){
			this->asteroidGLNormals.push_back(tri->normal.r);
			this->asteroidGLNormals.push_back(tri->normal.g);
			this->asteroidGLNormals.push_back(tri->normal.b);
		}
	}
}

void Asteroid::deformAsteroid(vector<Point>* points){
	//srand(time(NULL));
	for(int i = 0; i < points->size(); i++){
		points->data()[i].moveInRandomDirection(1.0f);
	}
}

void Asteroid::createIcosahedronPoints(){
	GLfloat reductionFactor = 1;
	GLfloat baseSize = 1;

	GLfloat t = (1.0 + sqrt(5.0)) / 2.0;
	
	t = reductionFactor * t;
	baseSize = baseSize * reductionFactor;
	
	this->asteroidPoints.push_back(Point(-baseSize,  t, 0));
	this->asteroidPoints.push_back(Point( baseSize,  t, 0));
	this->asteroidPoints.push_back(Point(-baseSize, -t, 0));
	this->asteroidPoints.push_back(Point( baseSize, -t, 0));
	
	this->asteroidPoints.push_back(Point( 0, -baseSize,  t));
	this->asteroidPoints.push_back(Point( 0,  baseSize,  t));
	this->asteroidPoints.push_back(Point( 0, -baseSize, -t));
	this->asteroidPoints.push_back(Point( 0,  baseSize, -t));

	this->asteroidPoints.push_back(Point( t,  0, -baseSize));
	this->asteroidPoints.push_back(Point( t,  0,  baseSize));
	this->asteroidPoints.push_back(Point(-t,  0, -baseSize));
	this->asteroidPoints.push_back(Point(-t,  0,  baseSize));
}

void Asteroid::createIcosahedronTriangles(){
	this->asteroidTriangles.push_back(Triangle( 0, 11,  5));
	this->asteroidTriangles.push_back(Triangle( 0,  5,  1));
	this->asteroidTriangles.push_back(Triangle( 0,  1,  7));
	this->asteroidTriangles.push_back(Triangle( 0,  7, 10));
	this->asteroidTriangles.push_back(Triangle( 0, 10, 11));

	this->asteroidTriangles.push_back(Triangle( 1,  5,  9));
	this->asteroidTriangles.push_back(Triangle( 5, 11,  4));
	this->asteroidTriangles.push_back(Triangle(11, 10,  2));
	this->asteroidTriangles.push_back(Triangle(10,  7,  6));
	this->asteroidTriangles.push_back(Triangle( 7,  1,  8));
	
	this->asteroidTriangles.push_back(Triangle( 3,  9,  4));
	this->asteroidTriangles.push_back(Triangle( 3,  4,  2));
	this->asteroidTriangles.push_back(Triangle( 3,  2,  6));
	this->asteroidTriangles.push_back(Triangle( 3,  6,  8));
	this->asteroidTriangles.push_back(Triangle( 3,  8,  9));
	
	this->asteroidTriangles.push_back(Triangle( 4,  9,  5));
	this->asteroidTriangles.push_back(Triangle( 2,  4, 11));
	this->asteroidTriangles.push_back(Triangle( 6,  2, 10));
	this->asteroidTriangles.push_back(Triangle( 8,  6,  7));
	this->asteroidTriangles.push_back(Triangle( 9,  8,  1));
}

int Asteroid::calculateMiddlePoint(int p1, int p2, vector<Point>* points){
	bool firstSmaller = p1 < p2;
	int64_t smallerIndex = firstSmaller ? p1 : p2;
	int64_t greaterIndex = firstSmaller ? p2 : p1;
	int64_t key = (smallerIndex << 32) + greaterIndex;

	map<int64_t, int>::iterator iter = middlePointCache.find(key);

	if(iter != middlePointCache.end()){
		//We found the element
		return iter->second;
	}

	Point* first = &((*points)[p1]);
	Point* second = &((*points)[p2]);

	Point middlePoint = Point((first->coords.r + second->coords.r)/2.0, 
							  (first->coords.g + second->coords.g)/2.0, 
							  (first->coords.b + second->coords.b)/2.0);
	int index = affixVertex(middlePoint, *first, points);
	middlePointCache.insert(make_pair(key, index));
	return index;
}

int Asteroid::affixVertex(Point middlePoint, Point derpPoint, vector<Point>* pointsVector){
	double derpLength = sqrt(derpPoint.coords.r*derpPoint.coords.r + 
						 derpPoint.coords.g*derpPoint.coords.g + 
						 derpPoint.coords.b*derpPoint.coords.b);
	double length = sqrt(middlePoint.coords.r*middlePoint.coords.r + 
						 middlePoint.coords.g*middlePoint.coords.g + 
						 middlePoint.coords.b*middlePoint.coords.b);
	double ration = derpLength / length;
	//ration = 1 / length;
	//length = length/2;
	(*pointsVector).push_back(Point(middlePoint.coords.r * ration, 
									middlePoint.coords.g * ration, 
									middlePoint.coords.b * ration));
	return (*pointsVector).size()-1;
}

void Asteroid::refineIcoSphere(){
	vector<Triangle> refined = vector<Triangle>();

	for(int i = 0; i < this->asteroidTriangles.size(); i++){
		Triangle tri = this->asteroidTriangles[i];
		
		int a = calculateMiddlePoint(tri.p1Index, tri.p2Index, &this->asteroidPoints);
		int b = calculateMiddlePoint(tri.p2Index, tri.p3Index, &this->asteroidPoints);
		int c = calculateMiddlePoint(tri.p3Index, tri.p1Index, &this->asteroidPoints);

		refined.push_back(Triangle(tri.p1Index, a, c));
		refined.push_back(Triangle(tri.p2Index, b, a));
		refined.push_back(Triangle(tri.p3Index, c, b));
		refined.push_back(Triangle(a, b, c));
	}
	this->asteroidTriangles = refined;
}

void Asteroid::calculateDeformedResultantRadius(){
	GLfloat maxRadius = BASE_RADIUS;
	for(int i = 0; i < this->asteroidPoints.size(); i++){
		Point p = this->asteroidPoints.data()[i];
		
		GLfloat currentRadius = sqrt(p.coords.r*p.coords.r + p.coords.g*p.coords.g + p.coords.b*p.coords.g);
		if(currentRadius > maxRadius){
			maxRadius = currentRadius;
		}
	}
	printf("Actual Radius: %f\n", maxRadius);
	this->deformedResultantRadius = maxRadius;
}