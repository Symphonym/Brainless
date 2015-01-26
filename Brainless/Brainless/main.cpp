#include <SFML\Graphics.hpp>
#include "ResourceLoader.h"
#include "Renderer.h"
#include "TileMap.h"
#include "Editor.h"
#include "Constants.h"
#include "TestClass.h"

int main()
{
	//sf::RenderWindow window(sf::VideoMode(800, 600), "Brainless");
	/*sf::CircleShape circle(100.f);
	circle.setFillColor(sf::Color::Green);*/


	Editor editor;
	editor.run();

	//fork test!!!

	/*TestClass test;
	test.testUpdate();*/

	// TEST CODE
	/*TileMap::TileMapLayout layout;
	for (int i = 0; i < 100; i++)
	{
		layout.push_back(std::vector<Tile::TileTypes>());
		for (int y = 0; y < 100; y++)
		{
			if ((i % 5 == 0 && y % 5 == 0) || i % 3 == 0)
				layout[i].push_back(Tile::Nothing);
			else
				layout[i].push_back(Tile::Ground);
		}
	}
	TileMap map(layout, Constants::TileSize);

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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			defView.move(0, -10);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			defView.move(0, 10);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			defView.move(-10, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			defView.move(10, 0);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			defView.zoom(1.01f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			defView.zoom(0.99f);

		window.setView(defView);

		window.clear(sf::Color::Black);
		map.draw(defView);
		//window.draw(circle);
		window.display();
	}*/
	

	return 0;
}