#include "WindowItem.h"
#include "Game.h"
#include "ResourceLoader.h"

WindowItem::WindowItem(bool smashed, int levelIndex, int id)
:
Item("Window", "WindowWholeItem", id),
m_levelIndex(levelIndex),
m_smashed(smashed)
{
	m_usable = false;
	m_useString = "I can't open it, it's sealed shut";
	m_renderingMode = RenderingModes::Behind;
	updateTexture();
}

void WindowItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);

	writer << m_smashed << std::endl;
}
void WindowItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);

	reader >> m_smashed;
	updateTexture();
}

void WindowItem::onUse(Game &game)
{
	if (m_smashed)
		game.changeLevelTransition(m_levelIndex);
}
bool WindowItem::onInteractedWith(Item &otherItem)
{
	if (otherItem.getName() == "Stone" && !m_smashed)
	{
		m_usable = true;
		m_smashed = true;
		updateTexture();
		// TODO PLAY SOUND
	}
	
	return false;
}

Item* WindowItem::clone()
{
	return new WindowItem(*this);
}

void WindowItem::updateTexture()
{
	if (m_smashed)
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("WindowSmashedItem"));
	else
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("WindowWholeItem"));
}