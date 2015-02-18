#include "ArcadeMachineItem.h"
#include "Game.h"
#include "StateMachine.h"
#include "ArcadeMachine.h"

ArcadeMachineItem::ArcadeMachineItem(int id)
:
Item("Arcade Machine", "ArcadeMachineItem", id)
{
	m_examineString = "It's an arcade machine, it appears to be in working condition";
	m_pickupString = "It's too heavy for me to carry";
	m_usable = true;
	m_renderingMode = RenderingModes::Behind;
}

void ArcadeMachineItem::onUse(Game &game)
{
	game.getMachine().pushState<ArcadeMachine>();
}


Item* ArcadeMachineItem::clone()
{
	return new ArcadeMachineItem(*this);
}