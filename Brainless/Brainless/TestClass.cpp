#include <SFML\Graphics.hpp>
#include "testClass.h"
#include "Player.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include <iostream>

using namespace std;

TestClass::TestClass() : unit(300, 300, 131, 73, 200, 200)
{
	ResourceLoader::instance().loadTexture("testImage", "test2.png");
	unit.setTexture(ResourceLoader::instance().retrieveTexture("testImage"));
}
void TestClass::testUpdate()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Brainless");
	sf::CircleShape circle(100.f);
	sf::RectangleShape rect(sf::Vector2f(10, 720));
	rect.setPosition(800, 0);
	circle.setFillColor(sf::Color::Green);

	sf::Clock clock;
	float deltaTime = clock.restart().asSeconds();

	int test=0;

	sf::View defView = window.getDefaultView();
	Renderer::instance().setTarget(window);
	while (window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
			defView.move(0, -1);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
			defView.move(0, 1);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
			defView.move(-1, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
			defView.move(1, 0);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
			defView.zoom(1.001f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			defView.zoom(0.999f);

		window.setView(defView);

		window.clear(sf::Color::Black);

		window.draw(circle);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) test = 0;
		window.draw(rect);
		if (test == 0)
		{
			unit.checkPlayerInput();
			unit.updateMovement(200, deltaTime);
			//collisiongrejis?	
		}
		unit.draw(); 

		Renderer::instance().executeDraws();
		window.display();
		
	}


}