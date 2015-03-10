#include "PuddleCableItem.h"
#include "ResourceLoader.h"
#include "Unit.h"
#include "ItemDatabase.h"
#include "Game.h"
#include "ParticleSystem.h"
#include "ScriptedZombie.h"

PuddleCableItem::PuddleCableItem(bool cableInPuddle, int id)
:
Item("Eletrical Puddle", "PuddleCableItem", "PuddleCableItem", id),
m_cableInPuddle(cableInPuddle)
{
	refreshStatus();
	m_lootable = false;
	m_collisionBounds = sf::FloatRect(15, 0, 105, 35);
	m_interactBounds = sf::FloatRect(15, 0, 105, 35);
	m_examineString = "I wish I could splash around without a care in the world and without being electrocuted.";
	m_useString = "There's nothing I can do with that";
}


void PuddleCableItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);

	writer << m_cableInPuddle << std::endl;
}
void PuddleCableItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);

	reader >> m_cableInPuddle;
	refreshStatus();
}

bool PuddleCableItem::onCollisionWithUnit(Unit &unit, Game &game)
{
	if (m_cableInPuddle)
	{
		if (unit.getUnitType() == Unit::UnitType::ID_CabinetZombie)
		{

			((ScriptedZombie&) unit).electricPuddle(game);
		}
		else
		{
			unit.takesDamage(sf::Vector2f(
				getPosition().x + getCollisionBounds().width / 2.f,
				getPosition().y + getCollisionBounds().height / 2.f));
		}
	}
	else
	{
		if (unit.getUnitType() == Unit::UnitType::ID_CabinetZombie)
		{
			((ScriptedZombie&)unit).safePuddle();
		}
	}
	return false;
}
bool PuddleCableItem::onInteractedWith(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Rubber Gloves" && m_cableInPuddle)
	{
		// Give player a cable
		game.lootItem(ItemDatabase::instance().extractItem(20));

		m_cableInPuddle = false;
		refreshStatus();
	}

	return false;
}

void PuddleCableItem::update(float deltaTime, Game &game)
{
	if (m_cableInPuddle)
	ParticleSystem::instance().addParticles(deltaTime * 600, getPosition() + sf::Vector2f(85,20), sf::Color().Yellow, sf::Vector2f(0.5f,0.5f), 
	sf::Vector2f(0,360),sf::Vector2f(0,10), sf::Vector2f(-100,100),sf::Vector2f(-100,100),sf::Vector2f(0,3));
}

Item* PuddleCableItem::clone()
{
	return new PuddleCableItem(*this);
}


void PuddleCableItem::refreshStatus()
{
	if (m_cableInPuddle)
	{
		m_examineString = "I wish I could splash around without a care in the world and without being electrocuted.";
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("PuddleCableItem"));
	}
	else
	{
		m_examineString = "Such an interesting, natural, phenomenon. Good thing I looked at it...";
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("PuddleItem"));
	}
}
