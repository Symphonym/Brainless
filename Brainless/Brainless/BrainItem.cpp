#include "BrainItem.h"
#include "Unit.h"
#include "ConversationBox.h"

BrainItem::BrainItem(int id)
:
Item("Brain","BrainItemInv", "BrainItem", id)
{
	m_lootable = true;
	m_examineString = "I wonder what sound it would make if I slammed it back into someones head.";
	m_useString = "I don't think I should do that";
	m_dialog.loadDialogFile("dialogues/brainNoSync.txt");
}

bool BrainItem::onInteractUnit(Unit &unit, Game &game)
{
	if (unit.getUnitType() != unit.ID_Player
		&& unit.getUnitType() != unit.ID_CabinetZombie
		&& unit.getUnitType() != unit.ID_ItemZombie
		&& unit.getUnitType() != unit.ID_NONE)
	{
		if (unit.getSyncID() == getSyncID())
		{
			return true;
		}
		else
		{
			sf::Vector2i onScreenPos = game.getWindow().mapCoordsToPixel(getPosition());

			m_dialog.resetDialog();
			ConversationBox::instance().setPosition(sf::Vector2f(onScreenPos.x, onScreenPos.y));
			ConversationBox::instance().setDialog(m_dialog);
			ConversationBox::instance().setShown(true);
		}
	}

	return false;
}

Item* BrainItem::clone()
{
	return new BrainItem(*this);
}