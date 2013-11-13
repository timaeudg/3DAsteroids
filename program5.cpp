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
		render.init(w, h);
		step = 2;
		outside = false;
		glm::vec3 camera = glm::vec3(0,0,0.5);
		glm::vec3 target = glm::normalize(glm::vec3(1,0,0))+camera;
		while (App->IsOpened())
		{			
			App->SetActive();

			float targetFrameTime = 1.0f/(float)TARGET_FPS;
			float sleepTime = targetFrameTime - App->GetFrameTime();
			if(sleepTime > 0)
				sf::Sleep(sleepTime);
			//Handle Camera Movement
			float delta = 3;
			if(input.IsKeyDown(sf::Key::Left)) {
				target = target - camera;
				glm::vec4 rot = glm::rotate(glm::mat4(1), delta, vec3(0,0,1))*(vec4(target,1)); 
				target = glm::normalize(vec3(rot[0], rot[1], rot[2]))+camera;
			}
			if(input.IsKeyDown(sf::Key::Right)) {
				target = target - camera;
				glm::vec4 rot = glm::rotate(glm::mat4(1), -delta, vec3(0,0,1))*(vec4(target,1)); 
				target = glm::normalize(vec3(rot[0], rot[1], rot[2]))+camera;
			}
			delta = 0.1;
			if(input.IsKeyDown(sf::Key::Up)) {
				vec3 oldCamera = camera;
				vec3 oldTarget = target;
				camera = camera+delta*(target - camera);
				target = glm::normalize(target - camera)+camera;
			}
			if(input.IsKeyDown(sf::Key::Down)) {
				camera = camera-delta*(target - camera);
				target = glm::normalize(target - camera)+camera;
			}

			if(input.IsKeyDown(sf::Key::Space)) {
				//TODO: We need to make it use the id that was picked here
				render.display(camera, target, true);
				float pickedColor[4];
				glReadPixels(RESOLUTION/2, RESOLUTION/2, 1, 1, GL_RGBA, GL_FLOAT, pickedColor);
				printf("Picked ID: %f\n", pickedColor[0]*100);
				//render.splitNextAsteroid(pickedColor[0]*256);
			} 

			render.display(camera, target, false);
			
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
			if(Event.Type == sf::Event::KeyPressed )
			{
				if(Event.Key.Code == sf::Key::Equal) {
					unsigned char *dest;
					unsigned int w = App->GetWidth();
					unsigned int h = App->GetHeight();
					glPixelStorei(GL_PACK_ALIGNMENT, 1);
					dest = (unsigned char *) malloc( sizeof(unsigned char)*w*h*3);
					glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, dest);
				
					FILE *file;
					file = fopen("_program1.ppm", "wb");
					fprintf(file, "P6\n%i %i\n%i\n", w, h, 255);
					for(int i=h-1; i>=0; i--)
						fwrite(&dest[i*w*3], sizeof(unsigned char), w*3, file);
					fclose(file);
					free(dest);
				}
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
