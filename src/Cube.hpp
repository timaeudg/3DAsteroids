#pragma once
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include "Object.hpp"

class Cube : virtual public Object { 
private:
	glm::mat4 transform;
	GLuint vertexBuffer;
	GLuint elementBuffer;

	static std::vector<glm::vec3> verts;
	static std::vector<glm::vec3> norms;
	static std::vector<GLuint> faces;
	
	static void genVerts();
	static void genNorms();
	static void genFaces();

public:
	
	Cube();
	~Cube();
	void step(float dt);
	bool render();
	glm::mat4 getTransform();
	glm::vec3 bb_min();
	glm::vec3 bb_max();


};
