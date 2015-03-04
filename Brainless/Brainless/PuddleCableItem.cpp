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
	refreshTexture();
	m_lootable = false;
	m_collisionBounds = sf::FloatRect(5, 0, 123, 30);
	m_interactBounds = sf::FloatRect(5, 0, 123, 30);
	m_examineString = "It's a puddle of rainwater.";
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
	refreshTexture();
}

bool PuddleCableItem::onCollisionWithUnit(Unit &unit, Game &game)
{
	if (m_cableInPuddle)
	{
		if (unit.getUnitType() == Unit::UnitType::ID_CabinetZombie)
		{

			((ScriptedZombie&) unit).electricPuddle();
		}
		else
		{
			unit.takesDamage(sf::Vector2f(
				getPosition().x + getSprite().getGlobalBounds().width / 2.f,
				getPosition().y + getSprite().getGlobalBounds().height / 2.f));
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
		refreshTexture();
	}

	return false;
}

void PuddleCableItem::update(float deltaTime, Game &game)
{
	if (m_cableInPuddle)
	ParticleSystem::instance().addParticles(1, getPosition() + sf::Vector2f(85,20), sf::Color().Yellow, sf::Vector2f(0.5f,0.5f), 
	sf::Vector2f(0,360),sf::Vector2f(0,10), sf::Vector2f(-100,100),sf::Vector2f(-100,100),sf::Vector2f(0,3));
}

Item* PuddleCableItem::clone()
{
	return new PuddleCableItem(*this);
}


void PuddleCableItem::refreshTexture()
{
	if (m_cableInPuddle)
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("PuddleCableItem"));
	else
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("PuddleItem"));
}
