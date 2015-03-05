#include "SpikeItem.h"
#include "Unit.h"
#include "ResourceLoader.h"

SpikeItem::SpikeItem(int id)
:
Item("Spikes", "SpikesNormal", "SpikesNormal", id),
m_spikesDampened(false)
{
	m_lootable = false;
	m_usable = false;
	m_examineString = "I really don't want to touch those, I don't want any piercings at the moment.";
	m_interactDistance = sf::Vector2f(250, 500);
	m_collisionBounds = sf::FloatRect(5, 77, 123, 51);
}

void SpikeItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);

	writer << m_spikesDampened << std::endl;
}
void SpikeItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);
	reader >> m_spikesDampened;
	if (m_spikesDampened)
		m_examineString = "The spikes should be safe now.";
	refreshTexture();
}

bool SpikeItem::onInteractedWith(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Stuffed Bag" && !m_spikesDampened)
	{
		m_spikesDampened = true;
		m_examineString = "The spikes should be safe now.";
		refreshTexture();
	}

	return false;
}
bool SpikeItem::onCollisionWithUnit(Unit &unit, Game &game)
{
	if (unit.getUnitType() == Unit::ID_Player)
	{
		if (!m_spikesDampened)
		{
			unit.takesDamage(sf::Vector2f(
				getPosition().x + getCollisionBounds().width / 2.f,
				getPosition().y + getCollisionBounds().height / 2.f), 3);
		}
	}

	return false;
}

Item* SpikeItem::clone()
{
	return new SpikeItem(*this);
}

bool SpikeItem::isDampened() const
{
	return m_spikesDampened;
}

void SpikeItem::refreshTexture()
{
	if (m_spikesDampened)
	{
		m_examineString = "It's a bunch of spikes, but they appear to be dampened by the paper";
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("SpikesDampened"));
	}
	else
	{
		m_examineString = "It's a bunch of sharp spikes, I should probably find a way to cushion them";
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("SpikesNormal"));
	}
}