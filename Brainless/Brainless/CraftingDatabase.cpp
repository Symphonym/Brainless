#include "CraftingDatabase.h"
#include "ItemDatabase.h"
#include <algorithm>

CraftingDatabase::CraftingDatabase()
{
	// TODO add crafting recipes
	addRecipe(13, { 4, 7 }); // Key + Twine = KeyTwine
	addRecipe(11, { 4, 3 }); // Key + Coin = CoinTwine
	addRecipe(19, { 17, 17, 0}); // Stick + Stick + DuctTape = LongStick
	addRecipe(26, { 22, 25, 0 }); // AABattery + Hammer + DuctTape = Thors Hammer
	addRecipe(42, { 43, 44 }); // Newspaper + Empty Bag = Stuffed Bag
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
bool CraftingDatabase::existsInRecipe(int id)
{
	for (auto itr = m_craftingRecipes.begin(); itr != m_craftingRecipes.end(); itr++)
	{
		for (std::size_t i = 0; i < itr->second.size(); i++)
		{
			if (id == itr->second[i])
				return true;
		}
	}

	return false;
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



