#include "ItemDatabse.h"

ItemDatabase::ItemDatabase()
{
	// TODO Testing item
	addItem(ItemPtr(new DefaultItem("GreenCube", 0)));
}

void ItemDatabase::addItem(ItemPtr itemPtr)
{
	m_items[itemPtr->getID()] = std::move(itemPtr);
}

ItemDatabase::ItemPtr ItemDatabase::extractItem(int id)
{
	return std::move(ItemPtr(m_items.at(id)->clone()));
}

ItemDatabase& ItemDatabase::instance()
{
	static ItemDatabase database;
	return database;
}
