#include "VendingMachineItem.h"
#include "ItemDatabase.h"
#include "Game.h"

VendingMachineItem::VendingMachineItem(const std::string &textureName, const IdVector &itemsWithin, int id)
:
Item("Vending Machine", textureName, id),
m_itemsWithin(itemsWithin)
{
	m_usable = true;
	m_renderingMode = RenderingModes::Behind;
	m_examineString = "It's a vending machine";
	m_pickupString = "It's way too heavy for me to carry";
}

void VendingMachineItem::onUse(Game &game)
{
	if (!m_payments.empty())
	{
		sf::Vector2f floorPosition(
			getPosition().x + getSprite().getGlobalBounds().width/2.f,
			getPosition().y + getSprite().getGlobalBounds().height);

		for (std::size_t i = 0; i < m_payments.size(); i++)
		{
			ItemDatabase::ItemPtr item;

			// If there's items left in the machine, push them out
			if (!m_itemsWithin.empty())
			{
				item = std::move(ItemDatabase::instance().extractItem(m_itemsWithin.back()));
				m_itemsWithin.pop_back();
			}
			// Otherwise output the coins, if it's a coin payment
			else if (m_payments[i])
				item = std::move(ItemDatabase::instance().extractItem(3));

			// Make sure there is an item
			if (!item || !item->isLootable())
				continue;

			item->setPosition(sf::Vector2f(floorPosition.x, floorPosition.y - item->getSprite().getGlobalBounds().height));

			int offset = std::rand() % 100;
			if (std::rand() % 2 == 0)
				offset *= -1;

			item->setPosition(sf::Vector2f(item->getPosition().x + offset, item->getPosition().y));

			game.getLevel().addItem(std::move(item));
		}
		m_payments.clear();
	}
}
bool VendingMachineItem::onInteractedWith(Item &otherItem)
{
	if (otherItem.getName() == "Coin")
		m_payments.push_back(true);
	else if (otherItem.getName() == "String & Coin")
	{
		bool noFalsePayment = true;
		for (std::size_t i = 0; i < m_payments.size(); i++)
		{
			if (!m_payments[i])
			{
				noFalsePayment = false;
				break;
			}
		}

		if (noFalsePayment)
			m_payments.push_back(false);
	}

	return false;
}

void VendingMachineItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);

	writer << m_payments.size() << std::endl; // Pending payments
	for (std::size_t i = 0; i < m_payments.size(); i++)
	{
		writer << m_payments[i];
		if (i != m_payments.size() - 1)
			writer << " ";
	}
	writer << std::endl;

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

	int payments = 0;
	reader >> payments;
	for (int i = 0; i < payments; i++)
	{
		bool value = false;
		reader >> value;
		m_payments.push_back(value);
	}

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

