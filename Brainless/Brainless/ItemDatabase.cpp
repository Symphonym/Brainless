#include "ItemDatabse.h"
#include "GhostItem.h"
#include "DoorItem.h"
#include "KeyItem.h"
#include "ChestItem.h"
#include "LadderItem.h"
#include "MagnifyingGlassItem.h"
#include "MovingPlatformItem.h"

ItemDatabase::ItemDatabase()
{
	// TODO Testing item
	addItem(ItemPtr(new DefaultItem("TestItem", 0, CombineData(1, 2))));
	addItem(ItemPtr(new DefaultItem("TestItem2", 1, CombineData(0, 2))));
	addItem(ItemPtr(new DefaultItem("TestItem3", 2, CombineData(4, 3))));
	addItem(ItemPtr(new DefaultItem("TestItem4", 3)));
	addItem(ItemPtr(new DefaultItem("TestItem5", 4, CombineData(2, 3))));
	addItem(ItemPtr(new GhostItem("dialog.txt", 5)));
	addItem(ItemPtr(new DoorItem(true, 6))); // Locked door
	addItem(ItemPtr(new KeyItem("KeyTest", 7))); // Key to said door
	addItem(ItemPtr(new ChestItem(false, { 0, 2, 7 }, 8)));
	addItem(ItemPtr(new LadderItem("LadderTest", 9)));
	addItem(ItemPtr(new MagnifyingGlassItem(10)));
	addItem(ItemPtr(new MovingPlatformItem(sf::Vector2f(100, 100), 1000, 11)));
}

void ItemDatabase::addItem(ItemPtr itemPtr)
{
	m_items[itemPtr->getID()] = std::move(itemPtr);
}

ItemDatabase::ItemPtr ItemDatabase::extractItem(int id)
{
	auto itr = m_items.find(id);

	if (itr == m_items.end())
		return ItemPtr();
	else
		return std::move(ItemPtr(m_items.at(id)->clone()));
}

ItemDatabase::ItemPtr ItemDatabase::extractItemByCount(int count)
{
	int curCount = 1;
	for (auto itr = m_items.begin(); itr != m_items.end(); itr++)
	{
		if (curCount == count)
			return std::move(ItemPtr(itr->second->clone()));

		curCount++;
	}

	return ItemPtr();
}

std::size_t ItemDatabase::getItemCount() const
{
	return m_items.size();
}

ItemDatabase& ItemDatabase::instance()
{
	static ItemDatabase database;
	return database;
}
