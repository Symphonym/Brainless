#include "ItemDatabase.h"
#include "GhostItem.h"
#include "DoorItem.h"
#include "KeyItem.h"
#include "ChestItem.h"
#include "LadderItem.h"
#include "MagnifyingGlassItem.h"
#include "MovingPlatformItem.h"
#include "DuctTapeItem.h"
#include "RubberGlovesItem.h"
#include "VendingMachineItem.h"
#include "CoinItem.h"
#include "TwineItem.h"
#include "CoinTwineItem.h"
#include "KeyTwineItem.h"
#include "WindowItem.h"
#include "StoneItem.h"
#include "LighterItem.h"
#include "ScissorLift.h"
#include "StickItem.h"
#include "DoubleStickItem.h"
#include "LongStickItem.h"
#include "CableItem.h"
#include "BrainItem.h"
#include "AABatteryItem.h"
#include "CarBatteryItem.h"
#include "DuctTapedBatteryItem.h"
#include "HammerItem.h"
#include "ThorsHammerItem.h"

ItemDatabase::ItemDatabase()
{
	addItem(ItemPtr(new DuctTapeItem(0)));
	addItem(ItemPtr(new RubberGlovesItem(1)));
	addItem(ItemPtr(new VendingMachineItem("VendingMachineItem1", { 0 }, 2)));
	addItem(ItemPtr(new CoinItem(3)));
	addItem(ItemPtr(new TwineItem(4)));
	addItem(ItemPtr(new GhostItem("dialog.txt", 5)));
	addItem(ItemPtr(new DoorItem(true, 6))); // Locked door
	addItem(ItemPtr(new KeyItem("KeyTest", 7))); // Key to said door
	addItem(ItemPtr(new ChestItem(false, { 0, 2, 7 }, 8)));
	addItem(ItemPtr(new LadderItem("LadderTest", 9)));
	addItem(ItemPtr(new MagnifyingGlassItem(10)));
	addItem(ItemPtr(new CoinTwineItem(11)));
	addItem(ItemPtr(new MovingPlatformItem(sf::Vector2f(0, 300), 800, 12)));
	addItem(ItemPtr(new KeyTwineItem(13)));
	addItem(ItemPtr(new WindowItem(false, 1, 14)));
	addItem(ItemPtr(new StoneItem(15)));
	addItem(ItemPtr(new LighterItem(16)));
	addItem(ItemPtr(new StickItem(17)));
	addItem(ItemPtr(new DoubleStickItem(18)));
	addItem(ItemPtr(new LongStickItem(19)));
	addItem(ItemPtr(new CableItem(20)));
	addItem(ItemPtr(new BrainItem(21)));
	addItem(ItemPtr(new AABatteryItem(22)));
	addItem(ItemPtr(new CarBatteryItem(23)));
	addItem(ItemPtr(new DuctTapedBatteryItem(24)));
	addItem(ItemPtr(new HammerItem(25)));
	addItem(ItemPtr(new ThorsHammerItem(26)));

	//addItem(ItemPtr(new ScissorLiftItem(sf::Vector2f(0, 100), 400, 17)));
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
