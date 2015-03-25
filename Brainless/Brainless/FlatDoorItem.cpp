#include "FlatDoorItem.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "Game.h"
#include "Notification.h"

FlatDoorItem::FlatDoorItem(bool locked, int levelIndex, int id)
:
Item("Door", "FlatClosed", "FlatClosed", id),
m_isLocked(locked),
m_levelIndex(levelIndex)
{
	m_usable = true;
	m_interactBounds = sf::FloatRect(15,35,100, 200);
}

bool FlatDoorItem::onInteractedWith(Item &otherItem, Game &game)
{
	if (getSyncID() == -1)
	{
		m_isLocked = false;
	}
	if (getSyncID() != -1 && m_isLocked && otherItem.getSyncID() == getSyncID() && otherItem.getName() == "Key")
	{
		m_isLocked = false;
		SoundPlayer::instance().playSound("item_door", getPosition());
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("FlatOpen"));
	}
	else if (m_isLocked && otherItem.getName() == "Key")
	{
		Notification::instance().write("The key doesn't seem to fit.");
	}
	return false;
}

void FlatDoorItem::onUse(Game &game)
{
	if (getSyncID() == -1)
	{
		m_isLocked = false;
	}
	// Toggle collision and open state
	if (!m_isLocked)
	{
		game.changeLevelTransition(m_levelIndex, false);
	}
}
void FlatDoorItem::onExamine()
{
	if (m_isLocked)
		m_examineString = "It appears to be locked";
	else
	{
		if (m_isLocked)
			m_examineString = "It's an open door, not much about it.";
		else
			m_examineString = "It's a closed door, maybe I should open it.";
	}
}

void FlatDoorItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);
	writer << m_isLocked << std::endl;
}
void FlatDoorItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);
	reader >> m_isLocked;

	if (!m_isLocked)
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("FlatOpen"));
	else
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("FlatClosed"));
}

void FlatDoorItem::draw()
{
	Renderer::instance().drawBehind(getSprite());
}

Item* FlatDoorItem::clone()
{
	return new FlatDoorItem(*this);
}