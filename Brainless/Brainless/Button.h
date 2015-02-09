#ifndef INCLUDED_BUTTON_H
#define INCLUDED_BUTTON_H

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "GUIElement.h"

class Button : public GUIElement
{
public:
	Button(sf::Texture &notPressed, sf::Texture &pressed, const sf::Vector2f &position, sf::RenderWindow *window);
	Button(sf::Texture &notPressed, sf::Texture &pressed, sf::IntRect placement, sf::RenderWindow *window);

	//Checks if the button is pressed and released
	virtual bool getReleased();
	virtual std::string getType();

	virtual void draw();

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