#include "HealthBar.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Utility.h"

HealthBar::HealthBar()
:
m_value(0),
m_maxValue(0),
m_width(65)
{
	setPosition(sf::Vector2f(0, 600));

	m_heartTexture = ResourceLoader::instance().retrieveTexture("heart");
	m_emptyTexture = ResourceLoader::instance().retrieveTexture("heartEmpty");

	for (std::size_t i = 0; i < m_sprites.size(); i++)
	{
		m_sprites[i].setPosition(m_position.x + m_width*i, m_position.y);
	}
}

void HealthBar::setPosition(const sf::Vector2f &position)
{
	m_position = position + sf::Vector2f(60,65);
}
void HealthBar::update(Player &player)
{
	m_value = player.getHealth();
	m_maxValue = player.getMaxHealth();


	for (int i = 0; i < m_maxValue; i++)
	{
		if (i < m_value)
			m_sprites[i].setTexture(m_heartTexture);
		else
			m_sprites[i].setTexture(m_emptyTexture);
	}
}

void HealthBar::draw()
{
	for (std::size_t i = 0; i < m_sprites.size(); i++)
	{
	    Renderer::instance().drawHUD(m_sprites[i]);
	}
}