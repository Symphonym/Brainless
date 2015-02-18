#ifndef INCLUDED_SCISSOR_LIFT_ITEM_H
#define INCLUDED_SCISSOR_LIFT_ITEM_H

#include "Item.h"
#include <SFML\Graphics.hpp>
#include "MovingPlatformItem.h"

class ScissorLiftItem : public Item
{
public:

	ScissorLiftItem(sf::Vector2f speed, float maxDistance, int id);

	virtual void onExamine();

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual bool onSyncedWith(Item &otherItem);

	virtual void update(float deltaTime, Game &game);
	virtual void draw();

	virtual void onPositionChanged();

	virtual Item* clone();

	sf::Vector2f getSpeed() const;
	sf::FloatRect getCollisionBounds() const;

private:

	bool m_isActive;
	sf::Vector2f m_speed;
	sf::Vector2f m_distanceMoved;
	float m_maxDistanceMoved;
	float m_distanceFromPlatform;
	MovingPlatformItem m_platform;
	sf::Sprite m_xTexture;
	sf::Sprite m_topTexture;
	float m_maxDistance;
	sf::IntRect m_xRectangle;
	int m_xSize = 200;
};

#endif