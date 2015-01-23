#include <SFML\Graphics.hpp>
#include "testClass.h"
#include "Object.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include <iostream>

TestClass::TestClass() : unit(0,0)
{
	ResourceLoader::instance().loadTexture("testImage", "test2.png");
	unit.setTexture(ResourceLoader::instance().retrieveTexture("testImage"));
}
void TestClass::testUpdate()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Brainless");
	sf::CircleShape circle(100.f);
	circle.setFillColor(sf::Color::Green);



	sf::View defView = window.getDefaultView();
	Renderer::instance().setTarget(window);
	while (window.isOpen())
	{
	sf::Event event;
	while (window.pollEvent(event))
	{
	if (event.type == sf::Event::Closed)
	window.close();

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	defView.move(0, -1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	defView.move(0, 1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	defView.move(-1, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	defView.move(1, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	defView.zoom(1.001f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	defView.zoom(0.999f);

	window.setView(defView);

	window.clear(sf::Color::Black);

	window.draw(circle);

	unit.draw();

	window.display();
	}


}