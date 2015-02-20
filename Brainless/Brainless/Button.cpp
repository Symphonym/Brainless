#include "Button.h"
#include "Renderer.h"

Button::Button(sf::Texture &notPressed, sf::Texture &pressed, const sf::Vector2f &position)
:
m_textureNotPressed(notPressed),
m_texturePressed(pressed)
{
	m_sizeRectangle = sf::IntRect(0, 0, notPressed.getSize().x, notPressed.getSize().y);
	m_position = position;
	m_hitBoxRect = sf::IntRect(m_position.x, m_position.y, notPressed.getSize().x, notPressed.getSize().y);
	m_mouseIsPressed = true;
}
Button::Button(sf::Texture &notPressed, sf::Texture &pressed, sf::IntRect placement)
:
m_textureNotPressed(notPressed),
m_texturePressed(pressed)
{
	m_sizeRectangle = sf::IntRect(0, 0, placement.width, placement.height);
	m_position = sf::Vector2f(placement.left, placement.top);
	m_hitBoxRect = sf::IntRect(m_position.x, m_position.y, placement.width, placement.height);
	m_mouseIsPressed = true;
}


bool Button::getReleased(const sf::Vector2i &mousePos)
{
	//The button must be pressed and released without the mouse leaving the button for the click to be registred.

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

std::string Button::getType()
{
	return "Button";
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

	Renderer::instance().drawHUD(m_sprite);
}