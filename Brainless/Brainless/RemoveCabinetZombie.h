#ifndef _REMOVECABINET_ZOMBIE
#define _REMOVECABINET_ZOMBIE
#include "Zombie.h"

class RemoveCabinetZombie : public Zombie
{
public:
	RemoveCabinetZombie(sf::Vector2f startPosition, int Texture, sf::Vector2f target);

	void updateTask(float deltaTime);

	virtual void onCollideWithItem(Item &item);
	//void electricPuddle();
	//void safePuddle();

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

protected:

private:
	Direction m_direction;
	sf::Vector2f m_target;
};

#endif