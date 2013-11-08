#pragma once
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>

class Object {
	public:
		virtual ~Object() {};
		virtual bool render() = 0;
		virtual glm::mat4 getTransform() = 0;
		virtual void step(float timeSinceLastUpdate) = 0; //update physics or animation
		virtual glm::vec3 bb_min() = 0; //the min <x, y, z> of the bounding box
		virtual glm::vec3 bb_max() = 0; //the max <x, y, z> of the bounding box
};
