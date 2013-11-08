#include <glm/gtc/matrix_transform.hpp>
#include "Cube.hpp"

using namespace glm;
using namespace std;

std::vector<glm::vec3> Cube::verts = vector<vec3>();
std::vector<glm::vec3> Cube::norms = vector<vec3>();
std::vector<GLuint> Cube::faces = vector<GLuint>();

Cube::Cube() {
	transform = mat4(1);
	Cube::genVerts();
	Cube::genNorms();
	Cube::genFaces();
}

Cube::~Cube() { } 

void Cube::step(float dt) {
	const float dtheta = 0.5f;
	transform = rotate(transform, dtheta*dt,  vec3(0, 1, 0));
}

mat4 Cube::getTransform() { return transform; }

bool Cube::render() {
	//Do some draw calls here
	return false;
}

vec3 Cube::bb_min() {
	vec3 min = vec3(255,255,255);
	for(unsigned int i = 0; i < Cube::verts.size(); i++) {
		for(int j = 0; j < 3; j++) {
			if(Cube::verts[i][j] < min[j])
				min[j] = Cube::verts[i][j];
		}
	}
	return min;
}

vec3 Cube::bb_max() {
	vec3 max = vec3(255,255,255);
	for(unsigned int i = 0; i < Cube::verts.size(); i++) {
		for(int j = 0; j < 3; j++) {
			if(Cube::verts[i][j] > max[j])
				max[j] = Cube::verts[i][j];
		}
	}
	return max;
}

void Cube::genVerts() {
	if(Cube::verts.empty()) {
		Cube::verts.push_back(vec3(-1,-1,-1));
		Cube::verts.push_back(vec3(-1,-1,1));
		Cube::verts.push_back(vec3(-1,1,-1));
		Cube::verts.push_back(vec3(-1,-1,1));
		Cube::verts.push_back(vec3(-1,1,1));
		Cube::verts.push_back(vec3(-1,1,-1));
		Cube::verts.push_back(vec3(-1,1,-1));
		Cube::verts.push_back(vec3(-1,1,1));
		Cube::verts.push_back(vec3(1,1,-1));
		Cube::verts.push_back(vec3(-1,1,1));
		Cube::verts.push_back(vec3(1,1,1));
		Cube::verts.push_back(vec3(1,1,-1));
		Cube::verts.push_back(vec3(1,1,-1));
		Cube::verts.push_back(vec3(1,1,1));
		Cube::verts.push_back(vec3(1,-1,-1));
		Cube::verts.push_back(vec3(1,1,1));
		Cube::verts.push_back(vec3(1,-1,1));
		Cube::verts.push_back(vec3(1,-1,-1));
		Cube::verts.push_back(vec3(1,-1,-1));
		Cube::verts.push_back(vec3(1,-1,1));
		Cube::verts.push_back(vec3(-1,-1,-1));
		Cube::verts.push_back(vec3(1,-1,1));
		Cube::verts.push_back(vec3(-1,-1,1));
		Cube::verts.push_back(vec3(-1,-1,-1));
	}
}

void Cube::genNorms() {
	if(Cube::norms.empty()) {
		Cube::norms.push_back(vec3(-1,0,0));
		Cube::norms.push_back(vec3(-1,0,0));
		Cube::norms.push_back(vec3(-1,0,0));
		Cube::norms.push_back(vec3(-1,0,0));
		Cube::norms.push_back(vec3(-1,0,0));
		Cube::norms.push_back(vec3(-1,0,0));
		Cube::norms.push_back(vec3(0,-1,0));
		Cube::norms.push_back(vec3(0,-1,0));
		Cube::norms.push_back(vec3(0,-1,0));
		Cube::norms.push_back(vec3(0,-1,0));
		Cube::norms.push_back(vec3(0,-1,0));
		Cube::norms.push_back(vec3(0,-1,0));
		Cube::norms.push_back(vec3(1,0,0));
		Cube::norms.push_back(vec3(1,0,0));
		Cube::norms.push_back(vec3(1,0,0));
		Cube::norms.push_back(vec3(1,0,0));
		Cube::norms.push_back(vec3(1,0,0));
		Cube::norms.push_back(vec3(1,0,0));
		Cube::norms.push_back(vec3(0,1,0));
		Cube::norms.push_back(vec3(0,1,0));
		Cube::norms.push_back(vec3(0,1,0));
		Cube::norms.push_back(vec3(0,1,0));
		Cube::norms.push_back(vec3(0,1,0));
		Cube::norms.push_back(vec3(0,1,0));
	}
}

void Cube::genFaces() {
	if(Cube::faces.empty()) {
		Cube::faces.push_back(0);
		Cube::faces.push_back(1);
		Cube::faces.push_back(2);
		Cube::faces.push_back(3);
		Cube::faces.push_back(4);
		Cube::faces.push_back(5);
		Cube::faces.push_back(6);
		Cube::faces.push_back(7);
		Cube::faces.push_back(8);
		Cube::faces.push_back(9);
		Cube::faces.push_back(10);
		Cube::faces.push_back(11);
		Cube::faces.push_back(12);
		Cube::faces.push_back(13);
		Cube::faces.push_back(14);
		Cube::faces.push_back(15);
		Cube::faces.push_back(16);
		Cube::faces.push_back(17);
		Cube::faces.push_back(18);
		Cube::faces.push_back(19);
		Cube::faces.push_back(20);
		Cube::faces.push_back(21);
		Cube::faces.push_back(22);
		Cube::faces.push_back(23);
	}
}
