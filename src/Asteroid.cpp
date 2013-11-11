#include "Asteroid.h"
#define REFINEMENT_LEVEL 2

vector<Point> Asteroid::icoPoints = vector<Point>();
vector<Triangle> Asteroid::icoTriangles = vector<Triangle>();
map<int64_t, int> Asteroid::middlePointCache = map<int64_t, int>();
bool Asteroid::randSeeded = false;


Asteroid::Asteroid(void)
{
	generateAsteroidBase();
	deformAndTransformBase();
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

void Asteroid::generateAsteroidBase(){
	if(this->icoPoints.size() == 0){
		//We haven't made the base of the asteroid, so we need to
		createIcosahedronPoints();
		createIcosahedronTriangles();
		printf("Number of Points Before: %d\n", Asteroid::icoPoints.size());
		//Now we have the icosahedron, we should make it a bit better though
		
		for(int i = 0; i < REFINEMENT_LEVEL; i++){
			deformAsteroid(&Asteroid::icoPoints);
			refineIcoSphere();
		}
		printf("Number of Points after: %d\n", Asteroid::icoPoints.size());
	}
}

void Asteroid::deformAndTransformBase(){
	copyBaseInformation();
	//deformAsteroid(&this->asteroidPoints);
	transformToGL();

	vector<GLuint> elements = vector<GLuint>();
	for(int i = 0; i < this->asteroidTriangles.size()*3; i++){
		elements.push_back(i);
	}
	this->elements = elements;
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

void Asteroid::copyBaseInformation(){
	for(int i = 0; i < Asteroid::icoPoints.size(); i++){
		this->asteroidPoints.push_back(Point(Asteroid::icoPoints[i].coords.r, Asteroid::icoPoints[i].coords.g, Asteroid::icoPoints[i].coords.b));
	}

	for(int i = 0; i < Asteroid::icoTriangles.size(); i++){
		this->asteroidTriangles.push_back(Triangle(Asteroid::icoTriangles[i].p1Index, Asteroid::icoTriangles[i].p2Index, Asteroid::icoTriangles[i].p3Index));
	}
}

void Asteroid::deformAsteroid(vector<Point>* points){
	for(int i = 0; i < points->size(); i++){
		randSeeded = points->data()[i].moveInRandomDirection(1.0f, randSeeded);
	}
}

void Asteroid::createIcosahedronPoints(){
	GLfloat reductionFactor = 1;
	GLfloat baseSize = 1;

	GLfloat t = (1.0 + sqrt(5.0)) / 2.0;
	
	t = reductionFactor * t;
	baseSize = baseSize * reductionFactor;
	
	Asteroid::icoPoints.push_back(Point(-baseSize,  t, 0));
	Asteroid::icoPoints.push_back(Point( baseSize,  t, 0));
	Asteroid::icoPoints.push_back(Point(-baseSize, -t, 0));
	Asteroid::icoPoints.push_back(Point( baseSize, -t, 0));
	
	Asteroid::icoPoints.push_back(Point( 0, -baseSize,  t));
	Asteroid::icoPoints.push_back(Point( 0,  baseSize,  t));
	Asteroid::icoPoints.push_back(Point( 0, -baseSize, -t));
	Asteroid::icoPoints.push_back(Point( 0,  baseSize, -t));
	
	Asteroid::icoPoints.push_back(Point( t,  0, -baseSize));
	Asteroid::icoPoints.push_back(Point( t,  0,  baseSize));
	Asteroid::icoPoints.push_back(Point(-t,  0, -baseSize));
	Asteroid::icoPoints.push_back(Point(-t,  0,  baseSize));
}

void Asteroid::createIcosahedronTriangles(){
	Asteroid::icoTriangles.push_back(Triangle( 0, 11,  5));
	Asteroid::icoTriangles.push_back(Triangle( 0,  5,  1));
	Asteroid::icoTriangles.push_back(Triangle( 0,  1,  7));
	Asteroid::icoTriangles.push_back(Triangle( 0,  7, 10));
	Asteroid::icoTriangles.push_back(Triangle( 0, 10, 11));
									 
	Asteroid::icoTriangles.push_back(Triangle( 1,  5,  9));
	Asteroid::icoTriangles.push_back(Triangle( 5, 11,  4));
	Asteroid::icoTriangles.push_back(Triangle(11, 10,  2));
	Asteroid::icoTriangles.push_back(Triangle(10,  7,  6));
	Asteroid::icoTriangles.push_back(Triangle( 7,  1,  8));
									 
	Asteroid::icoTriangles.push_back(Triangle( 3,  9,  4));
	Asteroid::icoTriangles.push_back(Triangle( 3,  4,  2));
	Asteroid::icoTriangles.push_back(Triangle( 3,  2,  6));
	Asteroid::icoTriangles.push_back(Triangle( 3,  6,  8));
	Asteroid::icoTriangles.push_back(Triangle( 3,  8,  9));
									 
	Asteroid::icoTriangles.push_back(Triangle( 4,  9,  5));
	Asteroid::icoTriangles.push_back(Triangle( 2,  4, 11));
	Asteroid::icoTriangles.push_back(Triangle( 6,  2, 10));
	Asteroid::icoTriangles.push_back(Triangle( 8,  6,  7));
	Asteroid::icoTriangles.push_back(Triangle( 9,  8,  1));
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

	for(int i = 0; i < Asteroid::icoTriangles.size(); i++){
		Triangle tri = Asteroid::icoTriangles[i];
		
		int a = calculateMiddlePoint(tri.p1Index, tri.p2Index, &Asteroid::icoPoints);
		int b = calculateMiddlePoint(tri.p2Index, tri.p3Index, &Asteroid::icoPoints);
		int c = calculateMiddlePoint(tri.p3Index, tri.p1Index, &Asteroid::icoPoints);

		refined.push_back(Triangle(tri.p1Index, a, c));
		refined.push_back(Triangle(tri.p2Index, b, a));
		refined.push_back(Triangle(tri.p3Index, c, b));
		refined.push_back(Triangle(a, b, c));
	}
	printf("Refined Size: %d\n", refined.size());
	printf("Ico Size: %d\n", Asteroid::icoTriangles.size());
	Asteroid::icoTriangles = refined;
}