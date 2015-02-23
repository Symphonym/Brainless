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
#include "SpiritFeatherItem.h"
#include "ArcadeMachineItem.h"
#include "ArcadeDiscItem.h"
#include "RpgGame.h"
#include "BeachParty.h"
#include "Boop.h"
#include "RobotAkeAttack.h"
#include "NoteGame.h"

ItemDatabase::ItemDatabase()
{
	addItem(ItemPtr(new DuctTapeItem(0)));
	addItem(ItemPtr(new RubberGlovesItem(1)));
	addItem(ItemPtr(new VendingMachineItem("VendingMachineItem1", { 0 }, 2)));
	addItem(ItemPtr(new CoinItem(3)));
	addItem(ItemPtr(new TwineItem(4)));
	addItem(ItemPtr(new GhostItem("dialog.txt", 5)));
	addItem(ItemPtr(new DoorItem(true, 6))); // Locked door
	addItem(ItemPtr(new KeyItem(7))); // Key to said door
	addItem(ItemPtr(new ChestItem(false, { 0, 2, 7 }, 8)));
	addItem(ItemPtr(new LadderItem("LadderTest", 9)));
	addItem(ItemPtr(new MagnifyingGlassItem(10)));
	addItem(ItemPtr(new CoinTwineItem(11)));
	addItem(ItemPtr(new MovingPlatformItem(sf::Vector2f(0, 800), 800, 12)));
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
	addItem(ItemPtr(new ScissorLiftItem(sf::Vector2f(0, 100), 200, 27)));
	addItem(ItemPtr(new SpiritFeatherItem(28)));
	addItem(ItemPtr(new ArcadeMachineItem(29)));


	// Add arcade game discs
	addItem(ItemPtr(new ArcadeDiscItem(30, [](ArcadeMachine &machine) -> ArcadeGame*
	{
		return new RpgGame(machine);
	})));
	addItem(ItemPtr(new ArcadeDiscItem(31, [](ArcadeMachine &machine) -> ArcadeGame*
	{
		return new BeachParty(machine);
	})));
	addItem(ItemPtr(new ArcadeDiscItem(32, [](ArcadeMachine &machine) -> ArcadeGame*
	{
		return new Boop(machine);
	})));
	addItem(ItemPtr(new ArcadeDiscItem(33, [](ArcadeMachine &machine) -> ArcadeGame*
	{
		return new RobotAkeAttack(machine);
	})));
	addItem(ItemPtr(new ArcadeDiscItem(34, [](ArcadeMachine &machine) -> ArcadeGame*
	{
		return new NoteGame(machine);
	})));
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
