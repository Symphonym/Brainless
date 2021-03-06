#include "WindowItem.h"
#include "Game.h"
#include "ResourceLoader.h"

WindowItem::WindowItem(bool smashed, int levelIndex, int id)
:
Item("Window","WindowWholeItem", "WindowWholeItem", id),
m_levelIndex(levelIndex),
m_smashed(smashed)
{
	m_usable = false;
	m_examineString = "An unboarded window, too dirty to see through though. This blows.";
	m_useString = "An unboarded window, too dirty to see through though.";
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
	if (m_smashed)
		m_examineString = "Oops! Leslie fled from my hand at incredible speed, come back Leslie!";
}

void WindowItem::onUse(Game &game)
{
	if (m_smashed)
		game.changeLevelTransition(game.getLevelIndex()+1,false);
}
bool WindowItem::onInteractedWith(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Stone" && !m_smashed)
	{
		SoundPlayer::instance().playSound("breakGlass_sound", getPosition());
		m_usable = true;
		m_smashed = true;
		m_examineString = "Oops! Leslie fled from my hand at incredible speed, come back Leslie!";
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

bool WindowItem::isActive()
{
	if (m_smashed)
	{
		return false;
	}
	else return true;
}
