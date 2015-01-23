#include <SFML\Graphics.hpp>
#include "ResourceLoader.h"
#include "TestClass.h"

int main()
{
	bool testActive = false; 
	TestClass test;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Brainless");
	sf::CircleShape circle(100.f);
	circle.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			if (event.key.code == sf::Keyboard::T)
				testActive = true;
		}

		if(testActive)test.testUpdate();

		window.clear(sf::Color::Black);

		window.draw(circle);
		window.display();
	}
	

	return 0;
}