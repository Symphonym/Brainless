#include "ArcadeMachineItem.h"
#include "Game.h"
#include "StateMachine.h"
#include "ArcadeMachine.h"
#include "ArcadeDiscItem.h"
#include "ItemDatabase.h"

ArcadeMachineItem::ArcadeMachineItem(int id)
:
Item("Arcade Machine","ArcadeMachineItem", "ArcadeMachineItem", id)
{
	m_examineString = "It's an arcade machine, it appears to be in working condition";
	m_pickupString = "It's too heavy for me to carry";
	m_usable = true;
	m_renderingMode = RenderingModes::Behind;
}


void ArcadeMachineItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);

	writer << m_loadedGameIDs.size() << std::endl;
	for (std::size_t i = 0; i < m_loadedGameIDs.size(); i++)
		writer << m_loadedGameIDs[i] << std::endl;
}
void ArcadeMachineItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);

	int idCount = 0;
	reader >> idCount;

	for (int i = 0; i < idCount; i++)
	{
		int id = 0;
		reader >> id;

		m_loadedGameIDs.push_back(id);
	}
}

void ArcadeMachineItem::onUse(Game &game)
{
	ArcadeMachine *machine = static_cast<ArcadeMachine*>(game.getMachine().pushState<ArcadeMachine>());

	for (std::size_t i = 0; i < m_loadedGameIDs.size(); i++)
	{
		ItemDatabase::ItemPtr item = ItemDatabase::instance().extractItem(m_loadedGameIDs[i]);

		if (item)
		{
			// Load games into arcade machine
			if (ArcadeDiscItem* discItem = dynamic_cast<ArcadeDiscItem*>(item.get()))
				machine->loadArcadeGame(discItem->createArcadeGame(*machine));
		}
	}
	
}

bool ArcadeMachineItem::onInteractedWith(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Arcade Game")
		m_loadedGameIDs.push_back(otherItem.getID());

	return false;
}

Item* ArcadeMachineItem::clone()
{
	return new ArcadeMachineItem(*this);
}