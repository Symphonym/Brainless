#include "ConductingItem.h"
#include "Notification.h"
#include "Game.h"

ConductingItem::ConductingItem(const std::string &itemName, const std::string &textureName, int id)
:
Item(itemName, textureName, id),
m_conducting(true)
{

}

void ConductingItem::onUse(Game &game)
{
	if (m_conducting && game.inventoryContains("Rubber Gloves"))
		onSafeUsage(game);
	else if (m_conducting)
		Notification::instance().write("It's conducting electricity, I have nothing from my bag to counter that");
	
	// The item is not conducting anymore
	else
		onSafeUsage(game);
}