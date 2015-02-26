#include "PuddleCableItem.h"
#include "ResourceLoader.h"
#include "Unit.h"
#include "ItemDatabase.h"
#include "Game.h"

PuddleCableItem::PuddleCableItem(bool cableInPuddle, int id)
:
Item("Eletrical Puddle", "PuddleCableItem", "PuddleCableItem", id),
m_cableInPuddle(cableInPuddle)
{
	refreshTexture();
	m_lootable = false;
	m_examineString = "It's a puddle of water";
	m_useString = "There's nothing I can do with that";
}


void PuddleCableItem::serialize(std::ofstream &writer) const
{
	writer << m_cableInPuddle << std::endl;
}
void PuddleCableItem::deserialize(std::ifstream &reader)
{
	reader >> m_cableInPuddle;
	refreshTexture();
}

bool PuddleCableItem::onCollisionWithUnit(Unit &unit, Game &game)
{
	if (m_cableInPuddle)
	{

	}
	//unit.takedamage
}
bool PuddleCableItem::onInteractedWith(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Rubber Gloves" && m_cableInPuddle)
	{
		// Give player a cable
		game.lootItem(ItemDatabase::instance().extractItem(20));

		m_collidable = false;
		refreshTexture();
	}
}

Item* PuddleCableItem::clone()
{
	return new PuddleCableItem(*this);
}


void PuddleCableItem::refreshTexture()
{
	if (m_cableInPuddle)
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("PuddleCableItem"));
	else
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("PuddleItem"));
}
