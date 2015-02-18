#ifndef INCLUDED_HEALTH_BAR_H
#define INCLUDED_HEALTH_BAR_H

#include <SFML\Graphics.hpp>
#include "Player.h"
#include <array>

class HealthBar
{
public:

	HealthBar();

	void setPosition(const sf::Vector2f &position);

	void update(Player &player);

	void draw();

private:

	sf::Texture m_heartTexture;
	sf::Texture m_emptyTexture;

	std::array<sf::Sprite, 3> m_sprites;

	sf::Vector2f m_position;

	float m_width;
	int m_value, m_maxValue;
};

#endif