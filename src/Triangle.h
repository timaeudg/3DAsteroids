#include "Point.h"
using namespace std;
class Triangle
{
public:
	int p1Index;
	int p2Index;
	int p3Index;
	glm::vec3 normal;

	Triangle(const int p1, const int p2, const int p3){
		this->p1Index = p1;
		this->p2Index = p2;
		this->p3Index = p3;
	}
	~Triangle(void);

	void recalculateNormal(Point p1, Point p2, Point p3){
		glm::vec3 first = p1.coords - p2.coords;
		glm::vec3 second = p1.coords - p3.coords;
		glm::vec3 normalized = glm::normalize(glm::cross(first, second));
		this->normal = normalized;
	}
};



