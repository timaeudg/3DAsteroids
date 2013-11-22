#ifndef __RENDERENGINE
#define __RENDERENGINE

#define ARENA_SIZE 10.0
#define NUMBER_OF_LARGE_ASTEROIDS 4
#define NUMBER_OF_MEDIUM_ASTEROIDS_PER_LARGE 2
#define NUMBER_OF_SMALL_ASTEROIDS_PER_MEDIUM 2
#define LARGE_ASTEROID_RADIUS 3.0
#define MEDIUM_ASTEROID_RADIUS 1.5
#define SMALL_ASTEROID_RADIUS 0.75

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLHelper.h"
#include "Asteroid.h"
#include "src/Cube.hpp"
using namespace glm;
class RenderEngine
{
public:
	RenderEngine()
	{
		initialized = false;
		this->P = glm::ortho(-1, 1, -1, 1);
	}

	~RenderEngine()
	{
		if(initialized)
		{
			// Clean up the buffers
			glDeleteBuffers(1, &positionBuffer);
			glDeleteBuffers(1, &elementBuffer);
			glDeleteBuffers(1, &colorBuffer);
			glDeleteBuffers(1, &normBuffer);
			glDeleteVertexArrays(1, &vertexArray);
		}
	}

	void init()
	{
		clock.Reset();
		setupGlew();
		setupShader();
		setupBuffers();
		initialized = true;
	}

	void display(vec3 camera = vec3(1,3,5), vec3 target = vec3(3,3,0), vec3 up = vec3(0,0,1), bool pickingEnabled = false)
	{
		glEnable(GL_DEPTH_TEST);
		//clear the old frame
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//use shader
		glUseProgram(shaderProg);
		
		//draw
		glBindVertexArray(vertexArray);

		mat4 C = mat4(1);
		C = glm::lookAt(camera, target,up);
		P = perspective(90.f, 16.0f/9.0f, 0.1f, 200.f);

		glUniformMatrix4fv(cameraSlot, 1, GL_FALSE, &C[0][0]);
		glUniformMatrix4fv(perspectiveSlot, 1, GL_FALSE, &P[0][0]);
		GLfloat t = clock.GetElapsedTime();
		glUniform1f(timeSlot, t);

		glm::mat4 T = glm::mat4(1);
		T =  glm::scale(T, glm::vec3(ARENA_SIZE, ARENA_SIZE, ARENA_SIZE));

		GLuint elementsSize = points.size();
		
		if(pickingEnabled){
//			printf("Picking is enabled\n");
			glUniform1i(pickingSlot, GL_TRUE);
		} else {
			glUniform1i(pickingSlot, GL_FALSE);
		}
		
		glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
		glUniform1f(idSlot, 0.0);
		glDrawElements(GL_TRIANGLES, arena.triangles.size(), GL_UNSIGNED_INT, 0);

		for(map<float, int>::iterator iterator = activeAsteroids.begin(); iterator != activeAsteroids.end(); iterator++){
			float asteroidID = iterator->first;
			int asteroidIndex = iterator->second;
			if(asteroids[asteroidIndex].isAlive()){
				int asteroidSize = asteroids[asteroidIndex].getElementArray().size();
				int drawOffset = arena.triangles.size() + asteroidIndex * asteroidSize;

				T = asteroids[asteroidIndex].getTransformMatrix(ARENA_SIZE, pickingEnabled);

				//printf("Asteroid ID: %f\n", asteroidID/256.0);
				glUniform1f(idSlot, asteroidID);

				glUniformMatrix4fv(matSlot, 1, GL_FALSE, &T[0][0]);
				glDrawElements(GL_TRIANGLES, asteroidSize, GL_UNSIGNED_INT, (void*)(drawOffset*sizeof(GLuint)));
			}
		}
		
		//cleanup
		glBindVertexArray(0);
		glUseProgram(0);
		checkGLError("display");
	}
	
	void reshape(int const & newWidth, int const & newHeight)
	{
		glViewport(0, 0, newWidth, newHeight);
	}

	void splitNextAsteroid(float id){
		if(asteroids[activeAsteroids[id]].isAlive()){
			asteroids[activeAsteroids[id]].kill();
			GLfloat killedSize = asteroids[activeAsteroids[id]].getRadius();

			int indexOfFirstChild;
			int indexOfSecondChild;

			if(killedSize == LARGE_ASTEROID_RADIUS && mediumAsteroidQueue.size() > 0){
				indexOfFirstChild = mediumAsteroidQueue.front();
				mediumAsteroidQueue.pop();
				indexOfSecondChild = mediumAsteroidQueue.front();
				mediumAsteroidQueue.pop();

				activeAsteroids.insert(make_pair(asteroids.data()[indexOfFirstChild].getID(), indexOfFirstChild));
				activeAsteroids.insert(make_pair(asteroids.data()[indexOfSecondChild].getID(), indexOfSecondChild));

				asteroids.data()[indexOfFirstChild].setStartingPoint(asteroids[activeAsteroids[id]].getPosition());
				asteroids.data()[indexOfSecondChild].setStartingPoint(asteroids[activeAsteroids[id]].getPosition());

			} else if(killedSize == MEDIUM_ASTEROID_RADIUS && smallAsteroidQueue.size() > 0){
				indexOfFirstChild = smallAsteroidQueue.front();
				smallAsteroidQueue.pop();
				indexOfSecondChild = smallAsteroidQueue.front();
				smallAsteroidQueue.pop();

				activeAsteroids.insert(make_pair(asteroids.data()[indexOfFirstChild].getID(), indexOfFirstChild));
				activeAsteroids.insert(make_pair(asteroids.data()[indexOfSecondChild].getID(), indexOfSecondChild));

				asteroids.data()[indexOfFirstChild].setStartingPoint(asteroids[activeAsteroids[id]].getPosition());
				asteroids.data()[indexOfSecondChild].setStartingPoint(asteroids[activeAsteroids[id]].getPosition());
			}
			
			map<float, int>::iterator iter = activeAsteroids.find(id);
			activeAsteroids.erase(iter);
			return;
		}
	}


private:
	bool initialized;
	Cube arena;
	vector<Asteroid> asteroids;
	map<float, int> activeAsteroids;
	queue<int> mediumAsteroidQueue;
	queue<int> smallAsteroidQueue;

	vector<GLfloat> points;
	vector<GLfloat> normals;
	vector<GLfloat> colors;
	vector<GLuint> elements;

	GLuint shaderProg;

	GLuint positionBuffer;
	GLuint elementBuffer;
	GLuint colorBuffer;
	GLuint normBuffer;
	GLuint vertexArray;

	GLint positionSlot;
	GLint matSlot;
	GLint perspectiveSlot;
	GLint cameraSlot;
	GLint colorSlot;
	GLint normSlot;
	GLint timeSlot;
	GLint pickingSlot;
	GLint idSlot;

	sf::Clock clock;
	
	unsigned int w;
	unsigned int h;
	
	glm::mat4 P;

	void setupGlew()
	{
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			exit(1);
		}
		printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
	}

	void setupShader()
	{
		char const * vertPath = "Shaders/simple.vert";
		char const * fragPath = "Shaders/simple.frag";

		shaderProg = ShaderManager::shaderFromFile(&vertPath, &fragPath, 1, 1);

		// Find out where the shader expects the data
		positionSlot = glGetAttribLocation(shaderProg, "pos");
		matSlot =      glGetUniformLocation(shaderProg, "M");
		perspectiveSlot = glGetUniformLocation(shaderProg, "P");
		cameraSlot = glGetUniformLocation(shaderProg, "C");
		colorSlot = glGetAttribLocation(shaderProg, "color");
		normSlot = glGetAttribLocation(shaderProg, "norm");
		timeSlot = glGetUniformLocation(shaderProg, "time");
		pickingSlot = glGetUniformLocation(shaderProg, "picking");
		idSlot = glGetUniformLocation(shaderProg, "ID");

		checkGLError("shader");
	}

	void setupBuffers()
	{
		arena = Cube();
		points = vector<GLfloat>(arena.vertices);
		normals = vector<GLfloat>(arena.normals);
		colors = vector<GLfloat>(arena.colorFloats);
		elements = vector<GLuint>(arena.triangles);

		unsigned int time_ui = unsigned int( time(NULL) );
		srand( time_ui );
		
		generateAsteroids(NUMBER_OF_LARGE_ASTEROIDS, LARGE_ASTEROID_RADIUS, &points, &normals, &colors, &elements);
		generateAsteroids(NUMBER_OF_LARGE_ASTEROIDS*NUMBER_OF_MEDIUM_ASTEROIDS_PER_LARGE, MEDIUM_ASTEROID_RADIUS, &points, &normals, &colors, &elements);
		generateAsteroids(NUMBER_OF_LARGE_ASTEROIDS*NUMBER_OF_MEDIUM_ASTEROIDS_PER_LARGE*NUMBER_OF_SMALL_ASTEROIDS_PER_MEDIUM, SMALL_ASTEROID_RADIUS, &points, &normals, &colors, &elements);
		
		for(int i = 0; i < asteroids.size(); i++){
			Asteroid a = asteroids[i];
			if(a.getRadius() == LARGE_ASTEROID_RADIUS){
				activeAsteroids.insert(make_pair(a.getID(), i));
			} else if(a.getRadius() == MEDIUM_ASTEROID_RADIUS){
				mediumAsteroidQueue.push(i);
			} else if (a.getRadius() == SMALL_ASTEROID_RADIUS){
				smallAsteroidQueue.push(i);
			}
		}

		/*
		for(int i = 0; i < asteroid.getVertices().size(); i++){
			points.push_back(asteroid.getVertices()[i]);
		}*/
		
		//setup the vertex array
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		//setup position buffer
		glGenBuffers(1, &positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(points.data()[0]), points.data(), GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, asteroid.getVertices().size()*sizeof(asteroid.getVertices().data()[0]), asteroid.getVertices().data(), GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(Wall::points), Wall::points, GL_STATIC_DRAW);
		glEnableVertexAttribArray(positionSlot);
		glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//setup normal Buffer
		printf("Norm Slot: %d\n", normSlot);
		glGenBuffers(1, &normBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(normals.data()[0]), normals.data(), GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, asteroid.getNormals().size()*sizeof(asteroid.getNormals()[0]), asteroid.getNormals().data(), GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(Wall::normals), Wall::normals, GL_STATIC_DRAW);
		glEnableVertexAttribArray(normSlot);
		glVertexAttribPointer(normSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		// Do the same thing for the color data
		printf("Color Slot: %d\n", colorSlot);
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, colors.size()*sizeof(colors.data()[0]), colors.data(), GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, asteroid.getColors().size()*sizeof(asteroid.getColors().data()[0]), asteroid.getColors().data(), GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(Wall::colors), Wall::colors, GL_STATIC_DRAW);
		glEnableVertexAttribArray(colorSlot);
		glVertexAttribPointer(colorSlot, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// now the elements
		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size()*sizeof(elements.data()[0]), elements.data(), GL_STATIC_DRAW);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Wall::elements), Wall::elements, GL_STATIC_DRAW);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, asteroid.getElementArray().size()*sizeof(asteroid.getElementArray()[0]), asteroid.getElementArray().data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
		
		checkGLError("setup");
	}

	void generateAsteroids(int numberOfAsteroids, GLfloat asteroidRadius, vector<GLfloat>* points, vector<GLfloat>* normals, vector<GLfloat>* colors, vector<GLuint>* elements){
		vector<GLfloat> asteroidPoints;
		vector<GLfloat> asteroidNormals;
		vector<GLfloat> asteroidColors;
		vector<GLuint>  asteroidElements;

		int asteroidSize = asteroids.size();
		//Make method for generating asteroids given size to make them, number to make, and vectors to put them in
		for(int i = 0; i<numberOfAsteroids; i++){
			int startingPosMax = ARENA_SIZE - asteroidRadius;
			GLfloat xPos = -startingPosMax + (float)rand()/((float)RAND_MAX/(startingPosMax-(-startingPosMax)));
			GLfloat yPos = -startingPosMax + (float)rand()/((float)RAND_MAX/(startingPosMax-(-startingPosMax)));
			GLfloat zPos = -startingPosMax + (float)rand()/((float)RAND_MAX/(startingPosMax-(-startingPosMax)));
			
			GLfloat radius = asteroidRadius;
			
			Asteroid asteroid = Asteroid(glm::vec3(xPos, yPos, zPos), (arena.vertices.size()/3) + asteroidSize*i, radius);

			asteroidSize = asteroid.getElementArray().size();

			asteroidPoints =   vector<GLfloat>(asteroid.getVertices());
			asteroidNormals =  vector<GLfloat>(asteroid.getNormals());
			asteroidColors =   vector<GLfloat>(asteroid.getColors());
			asteroidElements = vector<GLuint>(asteroid.getElementArray());
			
			points->insert(points->end(), asteroidPoints.begin(), asteroidPoints.end());
			elements->insert(elements->end(), asteroidElements.begin(), asteroidElements.end());
			normals->insert(normals->end(), asteroidNormals.begin(), asteroidNormals.end());
			colors->insert(colors->end(), asteroidColors.begin(), asteroidColors.end());

			asteroids.push_back(asteroid);
		}
	}
};

#endif