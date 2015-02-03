#ifndef INCLUDED_GHOST_ITEM_H
#define INCLUDED_GHOST_ITEM_H

#include "Item.h"
#include "DialogTree.h"

#include <string>

class GhostItem : public Item
{
public:

	explicit GhostItem(const std::string &dialogFile, int id);

	virtual void update(float deltaTime);
	virtual void draw();

	virtual void onUse(const sf::RenderWindow &window);

	virtual Item* clone();

private:

	// Dialog of the ghost
	DialogTree m_dialog;
};

#endif