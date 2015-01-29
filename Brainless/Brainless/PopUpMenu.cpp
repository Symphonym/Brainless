#include "PopUpMenu.h"

PopUpMenu::PopUpMenu(WorldButton &examine, WorldButton &use, WorldButton &pickUp, sf::Vector2f &position)
:
m_position(position)
{
	m_buttons[0] = examine;
	m_buttons[1] = use;
	m_buttons[2] = pickUp;

	m_hitbox = sf::IntRect(position.x, position.y,
		m_buttons[0].getHitbox().width + m_buttons[1].getHitbox().width + m_buttons[2].getHitbox().width,
		m_buttons[0].getHitbox().height + m_buttons[1].getHitbox().height + m_buttons[2].getHitbox().height);

	setPosition(position);

	m_isActive = false;
}

void PopUpMenu::activate()
{
	m_isActive = true;
}

void PopUpMenu::setPosition(sf::Vector2f pos)
{
	m_position = pos;

	m_buttons[0].setPosition(pos);
	m_buttons[1].setPosition(sf::Vector2f(pos.x, pos.y + m_buttons[0].getHitbox().height));
	m_buttons[2].setPosition(sf::Vector2f(pos.x, pos.y + m_buttons[0].getHitbox().height + m_buttons[1].getHitbox().height));

	m_hitbox.left = pos.x;
	m_hitbox.top = pos.y;
}

int PopUpMenu::getButtonPressed()
{
	if (m_isActive)
	{
		if (m_buttons[0].getReleased())
			return  1;
		else if (m_buttons[1].getReleased())
			return 2;
		else if (m_buttons[2].getReleased())
			return 3;
		else
			return 0;
	}
}

void PopUpMenu::update(sf::RenderWindow *window)
{
	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

	if (!m_hitbox.contains(sf::Vector2i(mousePos)))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			m_isActive = false;
		}
	}
}

void PopUpMenu::draw()
{
	if (m_isActive)
	{
		for (int i = 0; i < m_buttons.size(); i++)
		{
			m_buttons[i].draw();
		}
	}
}