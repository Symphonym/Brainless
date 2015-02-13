#include "ElectricalItem.h"

EletricalItem::EletricalItem(const std::string &itemName, const std::string &textureName, int id)
:
Item(itemName, textureName, id),
m_hasEletricity(false)
{

}

void EletricalItem::onUse(Game &game)
{
	if (m_hasEletricity)
		onUseOn(game);
	else
		onUseOff(game);
}

bool EletricalItem::onInteractedWith(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Cable" && !m_hasEletricity)
		m_hasEletricity = true;

	return false;
}
