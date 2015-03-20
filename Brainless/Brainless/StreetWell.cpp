#include "StreetWell.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "Notification.h"
#include "ItemDatabase.h"
#include "Game.h"

StreetWell::StreetWell(int id)
:
Item("StreetWell", "StreetWellItem", "StreetWellItem", id),
m_isKey(true)
{
	m_usable = true;
}

bool StreetWell::onInteractedWith(Item &otherItem, Game &game)
{
	if (getSyncID() != -1 && m_isKey && otherItem.getSyncID() == getSyncID() && otherItem.getName() == "Magnet & Twine")
	{
		//addkey
		game.lootItem(std::move(ItemDatabase::instance().extractItem(7)));
		m_isKey = false;
		
	}
	else if (m_isKey && otherItem.getName() == "Magnet & Twine")
	{
		Notification::instance().write("What trickery is this, the key doesn't seem to attach to the magnet.");
	}
	
	return false;
}

bool StreetWell::isActive()
{
	if(m_isKey)
		return false;
	else 
		return true;
}

void StreetWell::onUse(Game &game)
{
	// Toggle collision and open state
	if (m_isKey)
	{
		Notification::instance().write("I can't reach it!");
	}
	else
	{
		Notification::instance().write("I don't see anything of use in the streetwell.");
	}

}
void StreetWell::onExamine()
{
	if (m_isKey)
		m_examineString = "I see something shimmering down the well.";
	else
	{
		m_examineString = "I can almost see the ground down in the sewers, surely that was not a rat scurrying past right there? Probably Cthulhu.";
	}
}

void StreetWell::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);
	writer << m_isKey << std::endl;
}
void StreetWell::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);
	reader >> m_isKey;
}

void StreetWell::draw()
{
	Renderer::instance().drawDepth(getSprite());
}

Item* StreetWell::clone()
{
	return new StreetWell(*this);
}
