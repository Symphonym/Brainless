#include "DoorItem.h"
#include "ResourceLoader.h"
#include "Renderer.h"

DoorItem::DoorItem(bool locked, int id)
:
Item("Door", "DoorClosed", id),
m_isOpen(false),
m_isLocked(locked)
{
	m_usable = true;
	m_collidable = true;

	m_collisionSize = sf::Vector2f(50, 128);
	m_collisionOffset = sf::Vector2f(40, 0);
}

bool DoorItem::onInteractedWith(Item &otherItem)
{
	if (m_isLocked && otherItem.getSyncID() == getSyncID())
	{
		m_isLocked = false;
		// TODO Play unlock sound?
	}

	// The door will not get destroyed
	return false;
}

void DoorItem::onUse(Game &game)
{
	// Toggle collision and open state
	if (!m_isLocked)
	{
		m_isOpen = !m_isOpen;
		m_collidable = !m_collidable;

		if (m_isOpen)
			getSprite().setTexture(ResourceLoader::instance().retrieveTexture("DoorOpen"));
		else
			getSprite().setTexture(ResourceLoader::instance().retrieveTexture("DoorClosed"));
	}


}
void DoorItem::onExamine()
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

void DoorItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);
	writer << m_isLocked << std::endl;
	writer << m_isOpen << std::endl;
}
void DoorItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);
	reader >> m_isLocked >> m_isOpen;

	if (m_isOpen)
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("DoorOpen"));
	else
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("DoorClosed"));
}

void DoorItem::draw()
{
	if (m_isOpen)
		Renderer::instance().drawAbove(getSprite());
	else
		Renderer::instance().drawDepth(getSprite());
}

Item* DoorItem::clone()
{
	return new DoorItem(*this);
}
