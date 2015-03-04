#ifndef _REMOVECABINET_ZOMBIE
#define _REMOVECABINET_ZOMBIE
#include "Zombie.h"
#include "DialogTree.h"
#include "Game.h"

class RemoveCabinetZombie : public Zombie
{
public:
	RemoveCabinetZombie(sf::Vector2f startPosition, int Texture, sf::Vector2f target);

	void updateTask(float deltaTime);

	virtual void onCollideWithItem(Item &item);
	void electricPuddle(Game &game);
	void safePuddle();

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

protected:

private:
	bool m_dialogShown = false;
	Direction m_direction;
	sf::Vector2f m_target;
	DialogTree m_dialog;
};

#endif