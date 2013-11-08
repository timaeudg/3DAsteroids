#pragma once
#include <vector>

class Scene {
	public:
		virtual ~Scene() {}
		virtual bool render() = 0; //Render the scene to the currently selected GL buffer
		virtual void step(float timeElapsedSinceLast) = 0; //Take one Physics step

};
