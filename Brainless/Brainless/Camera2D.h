#ifndef INCLUDED_CAMERA2D_H
#define INCLUDED_CAMERA2D_H

#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\View.hpp"
#include "SFML\Graphics\RenderWindow.hpp"

class Camera2D
{
public:
	Camera2D(sf::Vector2f center, sf::Vector2f size);

	//Update with WASD controlls and updates the mousePos variable
	void levelEditorUpdate(sf::RenderWindow &window);

	//Update that follows a target (like the player)
	void gameUpdate(sf::RenderWindow &window, sf::Vector2f target);

	//Mouse position relative to the game world
	sf::Vector2f mousePos;

private:
	sf::View m_view;
	sf::Vector2f m_center;
	sf::Vector2f m_size;
};

#endif