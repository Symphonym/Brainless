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
#include "WindowOverlayItem.h"
#include "StoneItem.h"
#include "LighterItem.h"
#include "ScissorLift.h"
#include "StickItem.h"
#include "LongStickItem.h"
#include "CableItem.h"
#include "BrainItem.h"
#include "AABatteryItem.h"
#include "CarBatteryItem.h"
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
#include "Turtle.h"
#include "ManholeItem.h"
#include "PuddleCableItem.h"
#include "ElectricalCabinetItem.h"
#include "RopeLadderItem.h"
#include "SpikeItem.h"
#include "StuffedPaperBagItem.h"
#include "NewspaperItem.h"
#include "EmptyBagItem.h"
#include "BookShelfItem.h"
#include "CrateItem.h"
#include "Lamp.h"
#include "StartDialougeItem.h"
#include "SpiritFeatherSpawnItem.h"
#include "LeverItem.h"
#include "LeverSideItem.h"
#include "SteelDoor.h"
#include "FlatDoorItem.h"
#include "Magnet.h"
#include "MagnetTwine.h"
#include "StreetWell.h"

ItemDatabase::ItemDatabase()
{
	addItem(ItemPtr(new DuctTapeItem(0)));
	addItem(ItemPtr(new RubberGlovesItem(1)));
	addItem(ItemPtr(new VendingMachineItem("VendingMachineItem1", { 0 }, 2)));
	addItem(ItemPtr(new CoinItem(3)));
	addItem(ItemPtr(new TwineItem(4)));
	addItem(ItemPtr(new GhostItem(5,0)));
	addItem(ItemPtr(new DoorItem(true, 6))); // Locked door
	addItem(ItemPtr(new FlatDoorItem(true, 3, 58))); // Flat door
	addItem(ItemPtr(new SteelDoor(true, 57))); // Locked door
	addItem(ItemPtr(new KeyItem(7))); // Key to said door
	addItem(ItemPtr(new LeverItem(true, 54)));
	addItem(ItemPtr(new LeverSideItem(true, 55)));
	addItem(ItemPtr(new ChestItem(false, { 0, 1 }, 8)));
	addItem(ItemPtr(new LadderItem("WoodLadder1", 9)));
	addItem(ItemPtr(new LadderItem("SteelLadder1", 51)));
	addItem(ItemPtr(new LadderItem("RopeLadder1", 52)));
	addItem(ItemPtr(new MagnifyingGlassItem(10)));
	addItem(ItemPtr(new CoinTwineItem(11)));
	addItem(ItemPtr(new MovingPlatformItem(sf::Vector2f(0, 400), 800, 12)));
	addItem(ItemPtr(new KeyTwineItem(13)));
	addItem(ItemPtr(new WindowItem(false, 1, 14)));
	addItem(ItemPtr(new StoneItem(15)));
	addItem(ItemPtr(new LighterItem(16)));
	addItem(ItemPtr(new StickItem(17)));
	//addItem(ItemPtr(new DoubleStickItem(18)));
	addItem(ItemPtr(new LongStickItem(19)));
	addItem(ItemPtr(new CableItem(20)));
	addItem(ItemPtr(new BrainItem(21)));
	addItem(ItemPtr(new AABatteryItem(22)));
	addItem(ItemPtr(new CarBatteryItem(23)));
	//addItem(ItemPtr(new DuctTapedBatteryItem(24)));
	addItem(ItemPtr(new HammerItem(25)));
	addItem(ItemPtr(new ThorsHammerItem(26)));
	addItem(ItemPtr(new ScissorLiftItem(sf::Vector2f(0, 100), 200, 27)));
	addItem(ItemPtr(new SpiritFeatherItem(28)));
	addItem(ItemPtr(new ArcadeMachineItem(29)));


	// Add arcade game discs
	addItem(ItemPtr(new ArcadeDiscItem(30, "Green", [](ArcadeMachine &machine) -> ArcadeGame*
	{
		return new RpgGame(machine);
	})));
	addItem(ItemPtr(new ArcadeDiscItem(31, "Violet", [](ArcadeMachine &machine) -> ArcadeGame*
	{
		return new BeachParty(machine);
	})));
	addItem(ItemPtr(new ArcadeDiscItem(32, "Violet", [](ArcadeMachine &machine) -> ArcadeGame*
	{
		return new Boop(machine);
	})));
	addItem(ItemPtr(new ArcadeDiscItem(33, "Red", [](ArcadeMachine &machine) -> ArcadeGame*
	{
		return new RobotAkeAttack(machine);
	})));
	addItem(ItemPtr(new ArcadeDiscItem(34, "Green", [](ArcadeMachine &machine) -> ArcadeGame*
	{
		return new NoteGame(machine);
	})));
	addItem(ItemPtr(new ArcadeDiscItem(37, "Blue", [](ArcadeMachine &machine) -> ArcadeGame*
	{
		return new Turtle(machine);
	})));

	addItem(ItemPtr(new ManholeItem(false, "SteelLadder5", 35)));
	addItem(ItemPtr(new ManholeItem(true, "SteelLadder5", 36)));
	addItem(ItemPtr(new PuddleCableItem(true, 38)));
	addItem(ItemPtr(new ElectricalCabinetItem({ 44, 11, 43 }, 39))); //items within f�r scriptTest, kan ta bort itemvectorn nu!
	addItem(ItemPtr(new RopeLadderItem("RopeLadder6", 40)));
	addItem(ItemPtr(new SpikeItem(41)));
	addItem(ItemPtr(new StuffedPaperBagItem(42)));
	addItem(ItemPtr(new NewspaperItem(43)));
	addItem(ItemPtr(new EmptyBagItem(44)));
	addItem(ItemPtr(new BookShelfItem(45)));
	addItem(ItemPtr(new CrateItem(46)));
	addItem(ItemPtr(new LadderItem("PlankLadder", 47, false)));
	//addItem(ItemPtr(new GhostItem("dialogues/LEVEL2.txt", 48)));
	addItem(ItemPtr(new Lamp(49)));
	addItem(ItemPtr(new StartDialougeItem(50)));
	addItem(ItemPtr(new SpiritFeatherSpawnItem(53)));
	addItem(ItemPtr(new WindowOverlayItem(56)));
	addItem(ItemPtr(new Magnet(59)));
	addItem(ItemPtr(new MagnetTwine(60)));
	addItem(ItemPtr(new StreetWell(61)));

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
