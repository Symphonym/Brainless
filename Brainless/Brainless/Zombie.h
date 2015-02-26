#ifndef _ZOMBIE
#define _ZOMBIE
#include "Unit.h"

class Zombie : public Unit
{
public:
	Zombie(sf::Vector2f startPosition, sf::Vector2f size, sf::Vector2f maxSpeed, sf::Vector2f spriteOffset, UnitType ID, int Texture);

	virtual bool onInteractedWith(Item &otherItem, Game &game);
	virtual void onCollideWidth(Unit *unit);

	void flash(sf::Vector2f cameraPos);
	void incrementTexture();
	int getTexture();

	bool collide(Unit *unit);
protected:
	int m_textureId;
private:
	float m_flashAlpha;
};

#endif