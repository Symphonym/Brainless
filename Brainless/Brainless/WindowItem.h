#ifndef INCLUDED_WINDOW_ITEM_H
#define INCLUDED_WINDOW_ITEM_H

#include "Item.h"

class WindowItem : public Item
{
public:

	WindowItem(bool smashed, int levelIndex, int id);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual void onUse(Game &game);
	virtual bool onInteractedWith(Item &otherItem, Game &game);

	virtual Item* clone();

private:

	void updateTexture();

	// The level the window leads to
	int m_levelIndex;

	// If the window is smashed or not
	bool m_smashed;
};

#endif