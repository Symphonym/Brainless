#ifndef INCLUDED_WORLD_BUTTON_H
#define INCLUDED_WORLD_BUTTON_H

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "GUIElement.h"

class WorldButton : public GUIElement
{
public:
	WorldButton(sf::Texture &notPressed, sf::Texture &pressed, sf::IntRect placement, sf::RenderWindow *window);

	//Checks if the button is pressed and released
	virtual bool getReleased();
	virtual std::string getType();
	sf::IntRect getHitbox();
	void setPosition(sf::Vector2f pos);

	void draw();

private:
	sf::IntRect m_sizeRectangle;
	sf::IntRect m_hitBoxRect;
	sf::Vector2f m_position;
	sf::Texture m_textureNotPressed;
	sf::Texture m_texturePressed;
	sf::Sprite m_sprite;
	sf::RenderWindow *m_window;
	bool m_buttonIsPressed;
	bool m_mouseIsPressed;
};

#endif