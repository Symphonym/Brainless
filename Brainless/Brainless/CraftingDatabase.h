#ifndef INCLUDED_CRAFTING_DATABASE_H
#define INCLUDED_CRAFTING_DATABASE_H

#include "Item.h"
#include <memory>
#include <vector>
#include <unordered_map>


class CraftingDatabase
{
public:

	typedef std::unique_ptr<Item> ItemPtr;
	typedef std::vector<int> IdVector;

	// The crafted item resulting from combining items of the specified id's is returned. Null if no
	// crafting combination was found for those id's
	ItemPtr craftItem(const IdVector &idVector);

	static CraftingDatabase &instance();

private:

	void addRecipe(int productID, const IdVector &ingredients);
	std::vector<std::pair<int, IdVector> > m_craftingRecipes;

	CraftingDatabase();
};

#endif