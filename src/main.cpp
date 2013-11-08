#include <iostream>
#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "Cubes.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	cout << "3D Asteroids\n\
By Eric Guilford, Scott Harris, Henrik Sohlberg and Devon Timaeus\n\
Copyright (c) 2013, released under the Simplified BSD License, see LICENCE for details\n";

	sf::RenderWindow App(sf::VideoMode(800,600), "Asteroids Alpha Test");
	
	while(App.IsOpened()) {
		sf::Event Event;
		while(App.GetEvent(Event)) {
			if(Event.Type == sf::Event::Closed)
				App.Close();
			if(Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Escape)
				App.Close();
		}
		
		App.Clear();
		//Do Drawing Here
		App.Display();
	}
	
	return EXIT_SUCCESS;
}
