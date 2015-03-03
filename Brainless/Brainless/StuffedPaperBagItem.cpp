#include "StuffedPaperBagItem.h"
#include "SpikeItem.h"

StuffedPaperBagItem::StuffedPaperBagItem(int id)
:
Item("Stuffed Bag", "StuffedBagItem", "StuffedBagItem", id)
{
	m_lootable = true;
	m_usable = false;
	m_useString = "That won't work";
	m_examineString = "A soft, stuffed bag, usefull for dampening my fall on things";
}

bool StuffedPaperBagItem::onInteract(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Spikes")
	{
		if (SpikeItem* spikes = dynamic_cast<SpikeItem*>(&otherItem))
		{
			if (!spikes->isDampened())
				return true;
			else
				return false;
		}
	}

	return false;
}

Item* StuffedPaperBagItem::clone()
{
	return new StuffedPaperBagItem(*this);
}