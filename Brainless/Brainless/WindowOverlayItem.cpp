#include "WindowOverlayItem.h"
#include "Game.h"
#include "ResourceLoader.h"

WindowOverlayItem::WindowOverlayItem(int id)
:
Item("WindowOverlay", "WindowOverlayItem", "WindowOverlayItem", id)
{
	m_usable = false;
	m_examineString = "An unboarded window, too dirty to see through though. This blows.";
	m_useString = "An unboarded window, too dirty to see through though.";
	m_renderingMode = RenderingModes::Behind;
}

void WindowOverlayItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);
}
void WindowOverlayItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);
}

bool WindowOverlayItem::onInteractedWith(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Stone")
	{
		SoundPlayer::instance().playSound("breakGlass_sound", getPosition());
		return true;
	}
	return false;
}

Item* WindowOverlayItem::clone()
{
	return new WindowOverlayItem(*this);
}