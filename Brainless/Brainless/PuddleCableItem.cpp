#include "PuddleCableItem.h"
#include "ResourceLoader.h"
#include "Unit.h"
#include "ItemDatabase.h"
#include "Game.h"
#include "ParticleSystem.h"
#include "ScriptedZombie.h"
#include "Renderer.h"

PuddleCableItem::PuddleCableItem(bool cableInPuddle, int id)
:
Item("Eletrical Puddle", "PuddleCableItem", "PuddleCableItem", id),
m_cableInPuddle(cableInPuddle),
m_animation()
{

	m_electricitySprite.setTexture(ResourceLoader::instance().retrieveTexture("Puddle_Electricity"));
	m_animation.loop(0, 3, 0, 5);
	m_electricitySprite.setTextureRect(m_animation.getRectangle(0));

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

			((ScriptedZombie&)unit).electricPuddle(game);
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
bool PuddleCableItem::onInteractedWith(Item &otherItem, Game &game) //ska ha id samma som p�len sen
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
bool PuddleCableItem::isActive()
{
	if (m_cableInPuddle)
		return true;
	else
		return false;
}

void PuddleCableItem::update(float deltaTime, Game &game)
{
	m_electricitySprite.setPosition(m_sprite.getPosition() + sf::Vector2f(-75, -80));

	if (m_cableInPuddle)
	{
		m_electricitySprite.setTextureRect(m_animation.getRectangle(deltaTime));

		/*	rip partiklar, men kan anv�nda dem till tors hammare ist�llet :D
		m_particleTime += deltaTime;
		while (m_particleTime > 0.01)
		{
		m_particleTime-= 0.01;
		ParticleSystem::instance().addParticles(1, getPosition() + sf::Vector2f(85, 20), sf::Color().Yellow, sf::Vector2f(0.3f, 0.3f),
		sf::Vector2f(0, 360), sf::Vector2f(0, 10), sf::Vector2f(-100, 100), sf::Vector2f(-100, 100), sf::Vector2f(0, 300));
		}*/
	}
	else
		m_electricitySprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
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

void PuddleCableItem::draw()
{
	Renderer::instance().drawDepth(m_sprite);

	Renderer::instance().drawAbove(m_electricitySprite);
}