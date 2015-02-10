#ifndef INCLUDED_ITEM_DATABASE_H
#define INCLUDED_ITEM_DATABASE_H

#include "Item.h"
#include <unordered_map>
#include <memory>

class ItemDatabase
{
public:

	typedef std::unique_ptr<Item> ItemPtr;

	// Extracts an item from the database by its ID
	// Returns a null pointer if the item was not found
	ItemPtr extractItem(int id);


	// Returns the "count"'th item in the database
	ItemPtr extractItemByCount(int count);

	std::size_t getItemCount() const;

	static ItemDatabase& instance();

private:


	ItemDatabase();

	// Add item to database
	void addItem(ItemPtr itemPtr);

	std::unordered_map<int, ItemPtr> m_items;
};

#endif