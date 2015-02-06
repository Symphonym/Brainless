#ifndef INCLUDED_MOVING_PLATFORM_ITEM_H
#define INCLUDED_MOVING_PLATFORM_ITEM_H

#include "Item.h"
#include <SFML\Graphics.hpp>

class MovingPlatformItem : public Item
{
public:

	MovingPlatformItem(sf::Vector2f speed, float maxDistance, int id);

	virtual void onExamine();

	virtual void update(float deltaTime, Game &game);

	virtual void draw();

	virtual Item* clone();

private:

	sf::Vector2f m_speed;
	sf::Vector2f m_distanceMoved;
	float m_maxDistanceMoved;
};

#endif