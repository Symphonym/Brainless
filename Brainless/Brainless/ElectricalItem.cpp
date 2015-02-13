#include "ElectricalItem.h"

EletricalItem::EletricalItem(const std::string &itemName, const std::string &textureName, int id)
:
Item(itemName, textureName, id),
m_batteryType(BatteryTypes::AA),
m_missingBatteries(0),
m_hasEletricity(false)
{

}

void EletricalItem::onUse(Game &game)
{
	if (m_hasEletricity && m_missingBatteries == 0)
		onUseOn(game);
	else
		onUseOff(game);
}

void EletricalItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);

	writer << m_hasEletricity << std::endl;
	writer << static_cast<int>(m_batteryType) << std::endl;
	writer << m_missingBatteries << std::endl;
}
void EletricalItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);

	reader >> m_hasEletricity;

	int batteryType = 0;
	reader >> batteryType;

	m_batteryType = static_cast<BatteryTypes>(batteryType);
	reader >> m_missingBatteries;
}

bool EletricalItem::onInteractedWith(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Cable" && !m_hasEletricity)
		m_hasEletricity = true;
	else if (m_missingBatteries > 0)
	{
		if (otherItem.getName() == "AA Battery" && m_batteryType == BatteryTypes::AA)
			--m_missingBatteries;
		else if (otherItem.getName() == "Car Battery" && m_batteryType == BatteryTypes::Car)
			--m_missingBatteries;
	}

	return false;
}

