#ifndef INCLUDED_BUTTON_H
#define INCLUDED_BUTTON_H

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "GUIElement.h"

class Button : public GUIElement
{
public:
	Button(sf::Texture &notPressed, sf::Texture &pressed, const sf::Vector2f &position);
	Button(sf::Texture &notPressed, sf::Texture &pressed, sf::IntRect placement);

	//Checks if the button is pressed and released
	virtual bool getReleased(const sf::RenderWindow &window);
	virtual std::string getType();

	virtual void draw();

private:
	sf::IntRect m_sizeRectangle;
	sf::IntRect m_hitBoxRect;
	sf::Vector2f m_position;
	sf::Texture m_textureNotPressed;
	sf::Texture m_texturePressed;
	sf::Sprite m_sprite;
	bool m_buttonIsPressed;
	bool m_mouseIsPressed;
};

#endif