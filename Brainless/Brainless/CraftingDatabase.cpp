#include "CraftingDatabase.h"
#include "ItemDatabase.h"
#include <algorithm>

CraftingDatabase::CraftingDatabase()
{
	// TODO add crafting recipes
	// TEST RECIPE
	addRecipe(15, { 10, 16 });
}

CraftingDatabase::ItemPtr CraftingDatabase::craftItem(const std::vector<int> &idVector)
{
	IdVector sortedVec = idVector;
	std::sort(sortedVec.begin(), sortedVec.end());

	for (auto &recipe : m_craftingRecipes)
	{
		// IdVectors are sorted so this should work
		if (recipe.second == sortedVec)
		{
			return ItemDatabase::instance().extractItem(recipe.first);
			break;
		}
	}

	return ItemPtr();
}

void CraftingDatabase::addRecipe(int productID, const std::vector<int> &ingredients)
{
	m_craftingRecipes.push_back(std::make_pair(productID, ingredients));
	std::sort(m_craftingRecipes.back().second.begin(),m_craftingRecipes.back().second.end());
}

CraftingDatabase &CraftingDatabase::instance()
{
	static CraftingDatabase inst;
	return inst;
}



