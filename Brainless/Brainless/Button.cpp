#include "Button.h"
#include "Renderer.h"

Button::Button(sf::Texture &notPressed, sf::Texture &pressed, sf::IntRect placement, sf::RenderWindow *window)
:
m_textureNotPressed(notPressed),
m_texturePressed(pressed),
m_window(window)
{
	m_sizeRectangle = sf::IntRect(0, 0, placement.width, placement.height);
	m_position = sf::Vector2f(placement.left, placement.height);
	m_hitBoxRect = sf::IntRect(m_position.x, m_position.y, placement.width, placement.height);
	m_mouseIsPressed = true;
}

bool Button::isClicked()
{
	//The button must be pressed and released without the mouse leaving the button for the click to be registred.

	sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
	bool wasPressed = m_buttonIsPressed;
	m_buttonIsPressed = false;

	if (m_hitBoxRect.contains(mousePos))
	{
		if (m_mouseIsPressed)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				m_buttonIsPressed = true;
				m_mouseIsPressed = true;
			}
			else
			{
				if (wasPressed && !m_buttonIsPressed)
				{
					return true;
				}
				else
				{
					return false;
				}
				m_buttonIsPressed = false;
				m_mouseIsPressed = false;
			}
		}

		if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			m_mouseIsPressed = true;
		}
	}
	else
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			m_mouseIsPressed = false;
		}
		else
		{
			m_mouseIsPressed = true;
		}
	}
	return false;
}

void Button::draw()
{
	if (m_buttonIsPressed)
	{
		m_sprite.setTexture(m_texturePressed);
	}
	else
	{
		m_sprite.setTexture(m_textureNotPressed);
	}

	m_sprite.setPosition(m_position);

	Renderer::instance().drawAbove(m_sprite);
}