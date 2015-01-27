#include <SFML\Graphics.hpp>
#include "testClass.h"
#include "Player.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include <iostream>

using namespace std;

TestClass::TestClass() 
{
	ResourceLoader::instance().loadTexture("testImage", "spritesheet.png");


}
void TestClass::testUpdate()
{
	Player m_unit(300, 400 - 90, 60, 90, 200, 600);
	m_unit.setTexture(ResourceLoader::instance().retrieveTexture("testImage"));

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Brainless");
	sf::CircleShape circle(100.f);
	sf::RectangleShape rect(sf::Vector2f(10, 720));
	rect.setPosition(800, 0);
	circle.setFillColor(sf::Color::Green);

	sf::Clock clock;
	float deltaTime = clock.restart().asSeconds();

	int test = 0;

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
			m_unit.checkPlayerInput();
			m_unit.updateMovement(600, deltaTime);
			m_unit.updateAnimation(deltaTime);
			//collisiongrejis?	
			if (m_unit.getInAir() && 400 < m_unit.getPositionY() + m_unit.getHeight())
			{
				m_unit.setStatus(false);
				m_unit.setPosition(m_unit.getPositionX(), 400 - m_unit.getHeight());
				m_unit.setSpeed(m_unit.getSpeedX(), 0);
				m_unit.setAcceleration(0, 0);
			}
		}
		m_unit.draw();

		Renderer::instance().executeDraws();
		window.display();

	}


}