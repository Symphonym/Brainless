#include "SteelDoor.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "Notification.h"

SteelDoor::SteelDoor(bool locked, int id)
:
Item("SteelDoor", "SteelDoorClosed", "SteelDoorClosed", id),
m_isOpen(false),
m_isLocked(locked)
{
	m_usable = true;
	m_collidable = true;
	if (id == -1) m_isLocked = false;
	m_collisionBounds = sf::FloatRect(48, 25, 25, 192);
}

bool SteelDoor::onInteractedWith(Item &otherItem, Game &game)
{
	if (getSyncID() != -1 && m_isLocked && otherItem.getSyncID() == getSyncID() && otherItem.getName() == "Key")
	{
		m_isLocked = false;
		SoundPlayer::instance().playSound("item_door", getPosition());
	}
	else if (m_isLocked && otherItem.getName() == "Key")
	{
		Notification::instance().write("The key doesn't seem to fit.");
	}
	// The door will not get destroyed
	return false;
}

bool SteelDoor::onSyncedWith(Item &otherItem)
{
	if (otherItem.getName() == "Lever")
	{
		m_isOpen = !m_isOpen;
		m_collidable = !m_collidable;
		if (m_isOpen)
		{
			m_collisionBounds = sf::FloatRect(m_collisionBounds.left, m_collisionBounds.top, 90, 192);
			getSprite().setTexture(ResourceLoader::instance().retrieveTexture("SteelDoorOpen"));
		}
		else
		{
			m_collisionBounds = sf::FloatRect(m_collisionBounds.left, m_collisionBounds.top, 25, 192);
			getSprite().setTexture(ResourceLoader::instance().retrieveTexture("SteelDoorClosed"));
		}
	}
	return false;
}


void SteelDoor::onUse(Game &game)
{
	// Toggle collision and open state
	if (!m_isLocked)
	{
		m_isOpen = !m_isOpen;
		m_collidable = !m_collidable;

		if (m_isOpen)
		{
			m_collisionBounds = sf::FloatRect(m_collisionBounds.left, m_collisionBounds.top, 90, 192);
			getSprite().setTexture(ResourceLoader::instance().retrieveTexture("SteelDoorOpen"));
		}
		else
		{
			m_collisionBounds = sf::FloatRect(m_collisionBounds.left, m_collisionBounds.top, 25, 192);
			getSprite().setTexture(ResourceLoader::instance().retrieveTexture("SteelDoorClosed"));
		}
	}
	else
	{
		Notification::instance().write("It's locked!");
	}

}
void SteelDoor::onExamine()
{
	if (m_isLocked)
		m_examineString = "It appears to be locked";
	else
	{
		if (m_isOpen)
			m_examineString = "It's an open door, not much about it.";
		else
			m_examineString = "It's a closed door, maybe I should open it.";
	}
}

void SteelDoor::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);
	writer << m_isLocked << std::endl;
	writer << m_isOpen << std::endl;
}
void SteelDoor::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);
	reader >> m_isLocked >> m_isOpen;

	if (m_isOpen)
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("SteelDoorOpen"));
	else
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("SteelDoorClosed"));
}

void SteelDoor::draw()
{
	if (m_isOpen)
		Renderer::instance().drawAbove(getSprite());
	else
		Renderer::instance().drawDepth(getSprite());
}

Item* SteelDoor::clone()
{
	return new SteelDoor(*this);
}
