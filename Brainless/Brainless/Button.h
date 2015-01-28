#ifndef INCLUDED_BUTTON_H
#define INCLUDED_BUTTON_H

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

class Button
{
public:
	Button(sf::Texture &notPressed, sf::Texture &pressed, sf::IntRect placement, sf::RenderWindow *window);

	//Checks if the button is pressed and released
	bool isClicked();

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