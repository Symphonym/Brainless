#ifndef _DROPITEM_ZOMBIE
#define _DROPITEM_ZOMBIE
#include "Zombie.h"
#include "DialogTree.h"
#include "Game.h"
#include "Level.h"

class DropItemZombie : public Zombie
{
public:
	DropItemZombie(sf::Vector2f startPosition, int Texture, Direction Direction, int syncID = -1);

	void updateTask(float deltaTime);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	bool releaseItems(Level* levelPtr, Game &game);
protected:

private:
	bool m_itemsGiven = false;
	DialogTree m_dialog;
	std::vector<int> m_itemsWithin;
};

#endif