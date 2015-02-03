#include "ItemDatabse.h"
#include "GhostItem.h"

ItemDatabase::ItemDatabase()
{
	// TODO Testing item
	addItem(ItemPtr(new DefaultItem("TestItem", 0, -1, CombineData(1, 2))));
	addItem(ItemPtr(new DefaultItem("TestItem2", 1, -1, CombineData(0, 2))));
	addItem(ItemPtr(new DefaultItem("TestItem3", 2, -1, CombineData(4, 3))));
	addItem(ItemPtr(new DefaultItem("TestItem4", 3)));
	addItem(ItemPtr(new DefaultItem("TestItem5", 4, -1, CombineData(2, 3))));
	addItem(ItemPtr(new GhostItem("dialog.txt", 5)));

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
