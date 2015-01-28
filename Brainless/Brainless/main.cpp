#include <SFML\Graphics.hpp>
#include "ResourceLoader.h"
#include "Renderer.h"
#include "TileMap.h"
#include "Editor.h"
#include "Constants.h"
#include "Animation.h"
#include "TestClass.h"

int main(int argc, const char *args[])
{
	//sf::RenderWindow window(sf::VideoMode(1280, 720), "Brainless");

	//if (argc == 1) // ONLY UNCOMMEN THESE IF's WHEN DEPLOYING PROGRAM, editor can then be accessed by running "game.exe editor"
	//{
	//	if (args[0] == "editor")
	//	{
			Editor editor;  
			editor.run();
	//	}
	//}

	/*	TestClass test;	//remove this when closer to finnishing program, used to test isolated player movement and animation.
		test.testUpdate(); */ 

	/*Renderer::instance().setTarget(window);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}
		window.clear(sf::Color::Black);
	
		window.display();
	}*/
	

	return 0;
}