#include "Zombie.h"
#include "Item.h"
#include "Game.h"
#include "ResourceLoader.h"

Zombie::Zombie(sf::Vector2f startPosition, sf::Vector2f size, sf::Vector2f maxSpeed, sf::Vector2f spriteOffset, UnitType ID, int Texture, int syncID)
:
m_textureId(Texture),
Unit(startPosition, size, maxSpeed, spriteOffset, ID, syncID),
m_isDamaging(true),
m_animState(anim_noAnimation)
{
	addTexture(ResourceLoader::instance().retrieveTexture("Zombie"));
	addTexture(ResourceLoader::instance().retrieveTexture("Zombie2"));
}


void Zombie::setDamaging(bool damaging)
{
	m_isDamaging = damaging;
}

bool Zombie::onInteractedWith(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Brain")
	{
		flash(game.getPlayer().getCameraPosition());
		game.addSavedZombie(1);
		return true;
	}
	return false;
}

bool Zombie::collide(Unit *unit)
{
	return unit->getCollisionRect().intersects(getCollisionRect());
}

void Zombie::incrementTexture()
{
	m_textureId++;
	if (m_textureId >= Constants::ZombieTextureCount)
		m_textureId = 0;

	m_sprite = &m_spriteSheets[std::trunc(m_textureId / 4)];
	m_animation.loop(0, 7, m_textureId % 4 * 2 + 1, 5);
};

int Zombie::getTextureID()
{
	return m_textureId;
};

int Zombie::getSyncID()
{
	return m_syncID;
}
void Zombie::onCollideWith(Unit *unit)
{
	if (m_isDamaging)
		unit->takesDamage(m_position);
}

void Zombie::updateAnimation(float deltaTime)
{
	m_sprite = &m_spriteSheets[std::trunc(m_textureId/4)];

	updateSpriteDirection();

	m_sprite->setTextureRect(m_animation.getRectangle(deltaTime));
}

void Zombie::animation_idle()
{
	if (m_animState != anim_idle)
	{
		m_animation.loop(0, 7, m_textureId%4 * 2 + 1, 5);
		m_animState = anim_idle;
	}
}

void Zombie::animation_walking()
{

	if (m_animState != anim_walking)
	{
		m_animation.loop(0, 7, m_textureId % 4 * 2, 10);
		m_animState = anim_walking;
	}

}

void Zombie::animation_idleSlow()
{
	if (m_animState != anim_idleSlow)
	{
		m_animation.loop(0, 7, m_textureId % 4 * 2 + 1, 3);
		m_animState = anim_idleSlow;
	}

}

void Zombie::animation_walkingSlow()
{
	if (m_animState != anim_walkingSlow)
	{
		m_animation.loop(0, 7, m_textureId % 4 * 2, 3);
		m_animState = anim_walkingSlow;
	}
}