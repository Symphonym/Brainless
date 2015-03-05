#include "VendingMachineItem.h"
#include "ItemDatabase.h"
#include "Game.h"

VendingMachineItem::VendingMachineItem(const std::string &textureName, const IdVector &itemsWithin, int id)
:
Item("Vending Machine",textureName, textureName, id),
m_itemsWithin(itemsWithin)
{
	m_usable = true;
	m_renderingMode = RenderingModes::Behind;
	m_examineString = "A modern treasure-chest, the sustainer of human life, the pinnacle of human invention, the- ...Oh they are out of Jupiter, I'll try another one then.";
	m_pickupString = "It's way too heavy for me to carry";
	m_interactBounds = sf::FloatRect(69, 21, 124, 220);
}


bool VendingMachineItem::onInteractedWith(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Coin" ||
		otherItem.getName() == "String & Coin")
	{
		sf::Vector2f floorPosition(
			getPosition().x + getSprite().getGlobalBounds().width / 2.f,
			getPosition().y + getSprite().getGlobalBounds().height);

		ItemDatabase::ItemPtr item;

		// If there's items left in the machine, push them out
		if (!m_itemsWithin.empty())
		{
			item = std::move(ItemDatabase::instance().extractItem(m_itemsWithin.back()));
			m_itemsWithin.pop_back();
		}

		// Otherwise output the coins, if it's a coin payment
		else if (otherItem.getName() == "Coin")
			item = std::move(ItemDatabase::instance().extractItem(3));

		// Make sure there is an item
		if (!item || !item->isLootable())
			return false;

		item->setPosition(sf::Vector2f(floorPosition.x, floorPosition.y - item->getSprite().getGlobalBounds().height));

		int offset = std::rand() % 100;
		if (std::rand() % 2 == 0)
			offset *= -1;

		item->setPosition(sf::Vector2f(item->getPosition().x + offset, item->getPosition().y));

		game.getLevel().addItem(std::move(item));

	}
	return false;
}

void VendingMachineItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);

	writer << m_itemsWithin.size() << std::endl; // Item count inside vending machine is saved
	for (std::size_t i = 0; i < m_itemsWithin.size(); i++)
	{
		writer << m_itemsWithin[i];
		if (i != m_itemsWithin.size() - 1)
			writer << " ";
	}
	writer << std::endl;
}
void VendingMachineItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);

	int itemsInside = 0;
	reader >> itemsInside;
	for (int i = 0; i < itemsInside; i++)
	{
		int value = 0;
		reader >> value;
		m_itemsWithin.push_back(value);
	}
}


Item* VendingMachineItem::clone()
{
	return new VendingMachineItem(*this);
}

