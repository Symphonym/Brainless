#include "ChestItem.h"
#include "ResourceLoader.h"
#include "ItemDatabase.h"
#include "Game.h"

ChestItem::ChestItem(bool locked, std::vector<int> itemsWithin, int id)
:
Item("Chest","ChestClosed", "ChestClosed", id),
m_itemsWithin(itemsWithin),
m_isLocked(locked),
m_isOpen(false)
{
	m_usable = true;
	m_renderingMode = RenderingModes::Behind;
	m_pickupString = "It's too heavy for me to carry";
	m_examineString = "It's a trashcan. I don't really have anything I want to get rid of at the moment.";
}

void ChestItem::onUse(Game &game)
{
	if (!m_isLocked)
	{
		m_isOpen = !m_isOpen;

		if (m_isOpen)
		{
			getSprite().setTexture(ResourceLoader::instance().retrieveTexture("ChestOpen"));

			sf::Vector2f floorPosition(getPosition().x + getSprite().getGlobalBounds().width/2, getPosition().y + getSprite().getGlobalBounds().height);

			// Spawn items when chest is opened
			for (std::size_t i = 0; i < m_itemsWithin.size(); i++)
			{
			

				ItemDatabase::ItemPtr item = std::move(ItemDatabase::instance().extractItem(m_itemsWithin[i]));
				
				// Make sure items in the chest are lootable
				if (!item->isLootable())
					continue;

				item->setPosition(sf::Vector2f(floorPosition.x, floorPosition.y - item->getSprite().getGlobalBounds().height));

				int offset = i * 60;

				item->setPosition(sf::Vector2f(item->getPosition().x + offset, item->getPosition().y));

				game.getLevel().addItem(std::move(item));
			}
			m_itemsWithin.clear();
		}
		else
			getSprite().setTexture(ResourceLoader::instance().retrieveTexture("ChestClosed"));
	}
}
bool ChestItem::onInteractedWith(Item &otherItem, Game &game)
{
	if (m_isLocked && otherItem.getSyncID() == getSyncID() && otherItem.getName() == "Key")
	{
		m_isLocked = false;
		// TODO play open sound?
	}

	return false;
}

void ChestItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);
	writer << m_isLocked << std::endl;
	writer << m_isOpen << std::endl;
}
void ChestItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);
	reader >> m_isLocked >> m_isOpen;

	if (m_isOpen)
	{
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("ChestOpen"));
		m_itemsWithin.clear();
	}
	else
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("ChestClosed"));
}

Item* ChestItem::clone()
{
	return new ChestItem(*this);
}