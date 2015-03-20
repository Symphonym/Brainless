#ifndef INCLUDED_STREETWELL_H
#define INCLUDED_STREETWELL_H
#include "Item.h"

class StreetWell : public Item
{
public:

	StreetWell(int id);

	// If MagnetTwine is used
	virtual bool onInteractedWith(Item &otherItem, Game &game);

	// To handle opening of door, if it is unlocked
	virtual void onUse(Game &game);
	virtual void onExamine();
	bool isActive();

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual void draw();

	virtual Item* clone();

private:
	bool m_isKey;

};

#endif