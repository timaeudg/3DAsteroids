#pragma once
#include "../glew/glew.h"
#include "../glm/glm.hpp"
#include <vector>


class Cube { 
private:
	glm::mat4 transform;
	GLuint vertexBuffer;
	GLuint elementBuffer;

	static std::vector<glm::vec3> verts;
	static std::vector<glm::vec3> norms;
	static std::vector<GLuint> faces;
	static std::vector<glm::vec3> colors;
	
	static void genVerts();
	static void genNorms();
	static void genFaces();
	static void genColors();

public:
	
	Cube();
	~Cube();
	void step(float dt);
	bool render();
	glm::mat4 getTransform();
	glm::vec3 bb_min();
	glm::vec3 bb_max();

	static std::vector<GLfloat> vertices;
	static std::vector<GLfloat> normals;
	static std::vector<GLuint> triangles;
	static std::vector<GLfloat> colorFloats;
};
