#include "ItemDatabse.h"

ItemDatabase::ItemDatabase()
{
	// Testing item
	addItem(ItemPtr(new Item("GreenCube", 0)));
}

void ItemDatabase::addItem(ItemPtr itemPtr)
{
	m_items[itemPtr->getID()] = std::move(itemPtr);
}

//ItemDatabase::ItemPtr ItemDatabase::extractItem(int id)
//{
	// TODO return ItemPtr(new Item(m_items.at(id)));
//}

ItemDatabase& ItemDatabase::instance()
{
	static ItemDatabase database;
	return database;
}
