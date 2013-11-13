//Replace with your usernames!
#define printusers() printf("Program by Devon Timaeus+Eric Guilford\n");

#define GLEW_STATIC
#include <stdio.h>
#include <stdlib.h>
#include "glew/glew.h"
#include <SFML/Window.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "ShaderManager.h"
#include "GLHelper.h"
#include "RenderEngine.h"
#include "Maze.h"
#include "MazeModel.h"

#define RESOLUTION 800
#define TARGET_FPS 30                // controls spin update rate
#define TIME_WINDOW 3                // number of frames motion is valid after release


class Program4
{
public:
	Program4(unsigned int const & w, unsigned int const & h)
	{
		App = new sf::Window(sf::VideoMode(RESOLUTION, RESOLUTION, 32), "program5");
		const sf::Input& input = App->GetInput();
		render.init();
		step = 2;
		firing = 0;
		outside = false;
		glm::vec3 camera = glm::vec3(0,0,0.5);
		glm::vec3 target = glm::normalize(glm::vec3(1,0,0))+camera;
		glm::vec3 up = glm::vec3(0,0,1);
		while (App->IsOpened())
		{			
			App->SetActive();

			float targetFrameTime = 1.0f/(float)TARGET_FPS;
			float sleepTime = targetFrameTime - App->GetFrameTime();
			if(sleepTime > 0)
				sf::Sleep(sleepTime);
			//Handle Camera Movement
			float delta = 0.1;
			if(input.IsKeyDown(sf::Key::A)) {
				camera = camera+delta*(target - camera);
				target = glm::normalize(target - camera)+camera;
			}
			if(input.IsKeyDown(sf::Key::Z)) {
				camera = camera-delta*(target - camera);
				target = glm::normalize(target - camera)+camera;
			}
			delta = 3;
			if(input.IsKeyDown(sf::Key::Left)) {
				target = target - camera;
				target = normalize(vec3(rotate(mat4(1), delta, up)*vec4(target.x, target.y, target.z, 1)));
				target += camera;
			}
			if(input.IsKeyDown(sf::Key::Right)) {
				target = target - camera;
				target = normalize(vec3(rotate(mat4(1), -delta, up)*vec4(target.x, target.y, target.z, 1)));
				target += camera;
			}
			if(input.IsKeyDown(sf::Key::Up)) {
				target = target - camera;
				mat4 rot = rotate(mat4(1), delta, cross(up, target));
				target = normalize(vec3(rot*vec4(target.x, target.y, target.z, 1)));
				up = normalize(vec3(rot*vec4(up.x,up.y,up.z,1)));
				target += camera;
			}
			if(input.IsKeyDown(sf::Key::Down)) {
				target = target - camera;
				mat4 rot = rotate(mat4(1), -delta, cross(up, target));
				target = normalize(vec3(rot*vec4(target.x, target.y, target.z, 1)));
				up = normalize(vec3(rot*vec4(up.x,up.y,up.z,1)));
				target += camera;
			}
			if(camera.x > 9) camera.x = 9;
			if(camera.x < -9) camera.x = -9;
			if(camera.y > 9) camera.y = 9;
			if(camera.y < -9) camera.y = -9;
			if(camera.z > 9) camera.z = 9;
			if(camera.z < -9) camera.z = -9;

			if(firing) {
				firing--;
				//TODO: We need to make it use the id that was picked here
					render.display(camera, target, up, true);
					if(firing == 5) {
					float pickedColor[4];
						glReadPixels(RESOLUTION/2, RESOLUTION/2, 1, 1, GL_RGBA, GL_FLOAT, pickedColor);
						float roundedID = floor(pickedColor[0]*255 + .5);
						printf("Picked ID: %f\n", roundedID);
						render.splitNextAsteroid(roundedID);
					}
			}

			
			if(!firing) render.display(camera, target, up, false);
			App->Display();
			handleEvents();
		}
	}
	
	~Program4()
	{
	}
	
private:
	sf::Window *App;
	RenderEngine render;
	unsigned int step;
	bool outside;
	int firing;

	void handleEvents()
	{
		const sf::Input& Input = App->GetInput();
		bool shiftDown = Input.IsKeyDown(sf::Key::LShift) || Input.IsKeyDown(sf::Key::RShift);
		sf::Event Event;
		while (App->GetEvent(Event))
		{
			if (Event.Type == sf::Event::Closed)
				App->Close();
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
				App->Close();
			
			// This is for grading your code. DO NOT REMOVE
			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Space) {
				firing = 6;
			} 
			if(Event.Type == sf::Event::KeyReleased && Event.Key.Code == sf::Key::Space) {
				firing = 0;
			}

			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::F12){
				render = RenderEngine();
				render.init();
			}

			
			if (Event.Type == sf::Event::Resized)
			{ glViewport(0, 0, Event.Size.Width, Event.Size.Height); }
		}
	}
};

int main(int argc, char ** argv)
{
	printusers();
	printf("Arrow keys to move, press 'm' for map view\n");
	
	/* check that there are sufficient arguments */
	unsigned int w;
	unsigned int h;
	if (argc < 3) {
		w = 3;
		h = 3;
		fprintf(stderr, "The width and height can be specified as command line arguments. Defaulting to %i %i\n", w, h);
	}
	else {
		w = atoi(argv[1]);
		h = atoi(argv[2]);
	}
	
	Program4 prog(w, h);
	
    return EXIT_SUCCESS;
}
