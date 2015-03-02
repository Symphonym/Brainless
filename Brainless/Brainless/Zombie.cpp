#include "Zombie.h"
#include "Item.h"
#include "Game.h"
#include "ParticleSystem.h"

Zombie::Zombie(sf::Vector2f startPosition, sf::Vector2f size, sf::Vector2f maxSpeed, sf::Vector2f spriteOffset, UnitType ID, int Texture)
:
m_textureId(Texture),
Unit(startPosition, size, maxSpeed, spriteOffset, ID),
m_isDamaging(true),
m_animState(anim_noAnimation)
{

}

void Zombie::flash(sf::Vector2f cameraPos)
{
	m_flashAlpha = 255;

	ParticleSystem::instance().addParticles(
		150,
		sf::Vector2f(cameraPos.x, cameraPos.y - 400),
		sf::Color::Blue,
		sf::Vector2f(0.5f, 1),
		sf::Vector2f(0, 360),
		sf::Vector2f(0, 100),
		sf::Vector2f(-800, 800),
		sf::Vector2f(-300, 300),
		sf::Vector2f(0, 5));

	ParticleSystem::instance().addParticles(
		150,
		sf::Vector2f(cameraPos.x, cameraPos.y - 400),
		sf::Color::Yellow,
		sf::Vector2f(0.5f, 1),
		sf::Vector2f(0, 360),
		sf::Vector2f(0, 100),
		sf::Vector2f(-700, 700),
		sf::Vector2f(-300, 300),
		sf::Vector2f(0, 5));

	ParticleSystem::instance().addParticles(
		150,
		sf::Vector2f(cameraPos.x + 640, cameraPos.y),
		sf::Color::Blue,
		sf::Vector2f(0.5f, 1),
		sf::Vector2f(0, 360),
		sf::Vector2f(0, 100),
		sf::Vector2f(-800, 0),
		sf::Vector2f(-500, 300),
		sf::Vector2f(0, 5));

	ParticleSystem::instance().addParticles(
		150,
		sf::Vector2f(cameraPos.x - 640, cameraPos.y),
		sf::Color::Blue,
		sf::Vector2f(0.5f, 1),
		sf::Vector2f(0, 360),
		sf::Vector2f(0, 100),
		sf::Vector2f(0, 800),
		sf::Vector2f(-500, 300),
		sf::Vector2f(0, 5));

	ParticleSystem::instance().addParticles(
		150,
		sf::Vector2f(cameraPos.x + 640, cameraPos.y),
		sf::Color::Yellow,
		sf::Vector2f(0.5f, 1),
		sf::Vector2f(0, 360),
		sf::Vector2f(0, 100),
		sf::Vector2f(-800, 0),
		sf::Vector2f(-500, 300),
		sf::Vector2f(0, 5));

	ParticleSystem::instance().addParticles(
		150,
		sf::Vector2f(cameraPos.x - 640, cameraPos.y),
		sf::Color::Yellow,
		sf::Vector2f(0.5f, 1),
		sf::Vector2f(0, 360),
		sf::Vector2f(0, 100),
		sf::Vector2f(0, 800),
		sf::Vector2f(-500, 300),
		sf::Vector2f(0, 5));
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
		// TODO add brain giving logic
		return true;
	}
}/*
void Zombie::onCollisionWithItem(Item &item)
{
	m_script.onCollisionWithItem(item, *this);
}*/

bool Zombie::collide(Unit *unit)
{
	return unit->getCollisionRect().intersects(getCollisionRect());
}

void Zombie::incrementTexture()
{
	m_textureId++;
	if (m_textureId >= Constants::ZombieTypeCount)
		m_textureId = 0;
	m_animation.loop(0, 7, m_textureId * 2, 3);
};

int Zombie::getTexture()
{
	return m_textureId;
};

void Zombie::onCollideWith(Unit *unit)
{
	if (m_isDamaging)
		unit->takesDamage(m_position - unit->getPosition());
}

void Zombie::updateAnimation(float deltaTime)
{
	m_sprite = &m_spriteSheets[0];

	updateSpriteDirection();

	m_sprite->setTextureRect(m_animation.getRectangle(deltaTime));
}

void Zombie::animation_idle()
{
	if (m_animState != anim_idle)
	{
		m_animation.loop(0, 7, m_textureId * 2 + 1, 5);
		m_animState = anim_idle;
	}
}

void Zombie::animation_walking()
{

	if (m_animState != anim_walking)
	{
		m_animation.loop(0, 7, m_textureId * 2, 10);
		m_animState = anim_walking;
	}

}

void Zombie::animation_idleSlow()
{
	if (m_animState != anim_idleSlow)
	{
		m_animation.loop(0, 7, m_textureId * 2 + 1, 3);
		m_animState = anim_idleSlow;
	}

}

void Zombie::animation_walkingSlow()
{
	if (m_animState != anim_walkingSlow)
	{
		m_animation.loop(0, 7, m_textureId * 2, 3);
		m_animState = anim_walkingSlow;
	}
}