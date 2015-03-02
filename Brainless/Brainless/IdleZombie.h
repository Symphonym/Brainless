#ifndef _IDLE_ZOMBIE
#define _IDLE_ZOMBIE
#include "Zombie.h"

class IdleZombie : public Zombie
{
public:
	IdleZombie(sf::Vector2f startPosition, Direction startDirection, int Texture);

	void updateTask(float deltaTime);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

protected:

private:
};

#endif