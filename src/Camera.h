/**
	I am not satisfied with how this camera is working, but I got stuck in my approach and
	could not come up with a better solution. 

	The solution is based on looking at a specific point on a wall. This
	makes it necessary to keep track of which wall the player is looking at
	(for example the Left, Right, Front, or Back) to be able to retrieve
	the corresponding camera vector.
*/

#define RESOLUTION 512
#define RESTRICTION 0.6f

class Camera
{
public:
	Camera()
	{
		e = glm::vec3(0,0,0);
		eyeWall = 'a';
		myX = 0.0f;
		myY = 0.0f;
	}

	~RenderEngine()
	{
	}

	// Move the camera, returning the new vector
	// e (eye position). 
	// Takes the camera vector c (reference point)
	// to move closer to that point
	glm::vec3 move(glm::vec3 c) {
		glm::vec3 tmp = c*0.01f;
		e += tmp;
		checkBorders();
		return e;
	}

	// Check to see if the eye is trying to
	// move to a non wishful position
	void checkBorders() {
		for (int i=0; i<3; i++) {
			if (e[i] >= RESTRICTION)
				e[i] = RESTRICTION;
			else if (e[i] <= -RESTRICTION)
				e[i] = -RESTRICTION;
		}
	}

	// Get the corresponding camera vector when looking at Back wall
	glm::vec3 getBack() {
		return glm::vec3(myX, 1, myY);
	}

	// Get the corresponding camera vector when looking at Front wall	
	glm::vec3 getFront() {
		return glm::vec3(-myX, -1, myY);	
	}

	// Get the corresponding camera vector when looking at Left wall
	glm::vec3 getLeft() {
		return glm::vec3(-1, myX, myY);
	}
	
	// Get the corresponding camera vector when looking at Right wall
	glm::vec3 getRight() {
		return glm::vec3(1, -myX, myY);
	}

	glm::vec3 getCameraVector() {
		switch(eyeWall) {
			case 'a':
				return getBack();
			case 'f':
				return getFront();
			case 'l':
				return getLeft();
			case 'r':
				return getRight();
		}
	}

	// Input is x = Event.MouseMove.X
	// and 		y = Event.MouseMove.Y
	//
	// and output is a camera vector c (reference point)
	glm::vec3 updateAndGetCameraVec3(int x, int y) {
		float changeX = (float)(RESOLUTION/2-x)/-300.0;
		float changeY = (float)(RESOLUTION/2-y)/300.0;
		updateCoordinates(changeX, changeY);
		return getCameraVector();
	}

	// Trim to prevent looking around "to fast"
	float trim(float val) {
		if(val >= 2.0)
			val = 1.99;
		else if(val <= -2.0)
			val = -1.99;
		return val;
	}

	void updateCoordinates(float x, float y) {
		x = trim(x);
		y = trim(y);
		myX += x;
		myY += y;
		if(myX <= -1.0f) {
			myX+=1.999;
			updateDirections('l');
		}
		else if(myX >= 1.0f) {
			myX-=1.999;
			updateDirections('r');
		}
		if(myY <= -1.0f) {
			myY = -1.0f;
		}
		else if(myY >= 1.0f) {
			myY = 1.0f;
		}
	}
	
	void updateDirections(char direction) {
		switch (eyeWall) {
			case 'a':
				return updateBack(direction);
			case 'f':
				return updateFront(direction);
			case 'l':
				return updateLeft(direction);
			case 'r':
				return updateRight(direction);
		}
	}

	void updateBack(char direction) {
		switch (direction) {
			case 'u':
					eyeWall = 't';
					break;			
			case 'd':
					eyeWall = 'b';
					break;			
			case 'l':
					eyeWall = 'l';
					break;	
			case 'r':
					eyeWall = 'r';
					break;	
		}
	}	

	void updateFront(char direction) {
		switch (direction) {
			case 'u':
					eyeWall = 't';
					break;			
			case 'd':
					eyeWall = 'b';
					break;			
			case 'l':
					eyeWall = 'r';
					break;	
			case 'r':
					eyeWall = 'l';
					break;	
		}
	}

	void updateLeft(char direction) {
		switch (direction) {
			case 'u':
					eyeWall = 't';
					break;			
			case 'd':
					eyeWall = 'b';
					break;			
			case 'l':
					eyeWall = 'f';
					break;	
			case 'r':
					eyeWall = 'a';
					break;	
		}
	}

	void updateRight(char direction) {
		switch (direction) {
			case 'u':
					eyeWall = 't';
					break;			
			case 'd':
					eyeWall = 'b';
					break;			
			case 'l':
					eyeWall = 'a';
					break;	
			case 'r':
					eyeWall = 'f';
					break;	
		}
	}

private:	
	float myX, myY;
	char eyeWall;
	glm::vec3 e;
};







/** Snippet


class Program4
{
public:
	int WIDTH;
	int HEIGHT;
	Program4()
	{
		int WIDTH = 960;
		int HEIGHT = 960;
		App = new sf::Window(sf::VideoMode(WIDTH, HEIGHT, 32), "program4");

		App->SetPosition(800,0);					//Set window position
		App->SetCursorPosition(WIDTH/2, HEIGHT/2);	//Set cursor position	
		App->ShowMouseCursor(false);				//Hide cursor

		while (App->IsOpened())
		{			
			App->SetActive();
			render.display();
			App->Display();
			handleEvents();
			App->SetCursorPosition(RESOLUTION/2, RESOLUTION/2);		// Center cursor position
		}
	}
	
private:
	sf::Window *App;
	RenderEngine render;

	void handleEvents()
	{
		const sf::Input& Input = App->GetInput();
		sf::Event Event;
		while (App->GetEvent(Event))
		{			
			if (Event.Type == sf::Event::MouseMoved) 
				render.updateCamera(Event.MouseMove.X, Event.MouseMove.Y);
			
			if (Event.Type == sf::Event::Closed)
				App->Close();

			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
				App->Close();
			
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Space))
				render.move();
		}
	}
};

*/
