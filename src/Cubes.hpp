#include <vector>
#include <glm/glm.hpp>
#include "Scene.hpp"
#include "Cube.hpp"


class Cubes: virtual public Scene {
private:
	Cube cube;
	glm::vec3 camera;
	glm::vec3 lookAt;
	glm::vec3 light;
public:
	bool render();
	void step(float dt);

};
