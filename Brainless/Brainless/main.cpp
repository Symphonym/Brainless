#include <SFML\Graphics.hpp>
#include "ResourceLoader.h"
#include "Renderer.h"
#include "TileMap.h"
#include "Editor.h"
#include "Constants.h"
#include "Animation.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Brainless");
	sf::CircleShape circle(100.f);
	circle.setFillColor(sf::Color::Green);

	ResourceLoader::instance().loadTexture("testSheet", "testSheet.png");
	Animation circleAnimation(100, 100);
	circleAnimation.animate(0, 3, 0, 1);

	/*Editor editor;
	editor.run();*/

	Renderer::instance().setTarget(window);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}

		circleAnimation.update();

		window.clear(sf::Color::Black);
		window.draw(circle);
		window.draw(sf::Sprite(ResourceLoader::instance().retrieveTexture("testSheet"), circleAnimation.rectangle));
		window.display();
	}
	

	return 0;
}