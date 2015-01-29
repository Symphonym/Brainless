#include "WorldButton.h"
#include "Renderer.h"

WorldButton::WorldButton(sf::Texture &notPressed, sf::Texture &pressed, sf::IntRect placement, sf::RenderWindow *window)
:
m_textureNotPressed(notPressed),
m_texturePressed(pressed),
m_window(window)
{
	m_sizeRectangle = sf::IntRect(0, 0, placement.width, placement.height);
	m_position = sf::Vector2f(placement.left, placement.top);
	m_hitBoxRect = sf::IntRect(m_position.x, m_position.y, placement.width, placement.height);
	m_mouseIsPressed = true;
}


bool WorldButton::getReleased()
{
	//The button must be pressed and released without the mouse leaving the button for the click to be registred.

	sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
	bool wasPressed = m_buttonIsPressed;
	m_buttonIsPressed = false;

	if (m_hitBoxRect.contains(sf::Vector2i(mousePos)))
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

sf::IntRect WorldButton::getHitbox()
{
	return m_hitBoxRect;
}

void WorldButton::setPosition(sf::Vector2f pos)
{
	m_position = pos;
	m_hitBoxRect.top = pos.y;
	m_hitBoxRect.left = pos.x;
}

std::string WorldButton::getType()
{
	return "Button";
}

void WorldButton::draw()
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