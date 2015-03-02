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
	m_collisionBounds = sf::FloatRect(5, 0, 123, 30);
	m_interactBounds = sf::FloatRect(5, 0, 123, 30);
	m_examineString = "It's a puddle of water";
	m_useString = "There's nothing I can do with that";
}


void PuddleCableItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);

	writer << m_cableInPuddle << std::endl;
}
void PuddleCableItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);

	reader >> m_cableInPuddle;
	refreshTexture();
}

bool PuddleCableItem::onCollisionWithUnit(Unit &unit, Game &game)
{
	if (m_cableInPuddle)
	{
		unit.takesDamage(sf::Vector2f(
			getPosition().x + getSprite().getGlobalBounds().width / 2.f,
			getPosition().y + getSprite().getGlobalBounds().height / 2.f));
	}

	return false;
}
bool PuddleCableItem::onInteractedWith(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Rubber Gloves" && m_cableInPuddle)
	{
		// Give player a cable
		game.lootItem(ItemDatabase::instance().extractItem(20));

		m_cableInPuddle = false;
		refreshTexture();
	}

	return false;
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
