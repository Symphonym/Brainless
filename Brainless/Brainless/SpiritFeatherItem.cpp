#include "SpiritFeatherItem.h"
#include "Unit.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"
#include "ParticleSystem.h"
#include "SoundPlayer.h"


SpiritFeatherItem::SpiritFeatherItem(int id)
:
Item("Spirit Feather","SpiritFeather", "SpiritFeather", id),
m_animation(100,100)
{
	m_collisionBounds = sf::FloatRect(15, 0, 40, 80);
	m_interactBounds = sf::FloatRect(15, 0, 40, 80);
	m_examineString = "For tickling, or for... collecting? I feel a sudden urge to stuff these in my backpack.";
	m_useString = "You try to eat the feather. It's not tasty. You spit it out and put it back into its hovering position.";
	m_animation.loop(0, 7, 0, 10);
	getSprite().setScale(0.7f,0.7f);
	getSprite().setTextureRect(m_animation.getRectangle(0));
}



bool SpiritFeatherItem::onCollisionWithUnit(Unit &unit, Game &game)
{
	// Delete item on player collision and add spirit power
	if (unit.getUnitType() == Unit::ID_Player)
	{
		ParticleSystem::instance().addParticles(
			50,
			sf::Vector2f(
				getPosition().x + getSprite().getGlobalBounds().width / 2.f,
				getPosition().y + getSprite().getGlobalBounds().height / 2.f),
			sf::Color::Cyan,
			sf::Vector2f(0.5f, 1),
			sf::Vector2f(0, 360),
			sf::Vector2f(0, 100),
			sf::Vector2f(-100, 100),
			sf::Vector2f(-100, 100));		
			//sf::Vector2f(-game.getPlayer().getSpeed().x / 100.f, -game.getPlayer().getSpeed().y / 100.f));

		SoundPlayer::instance().playSound("feather_sound", getPosition(), 20.0f);

		game.getSpiritBar().addValue(Constants::SpiritFeatherWorth);


		return true;
	}
	else
		return false;
}

void SpiritFeatherItem::update(float deltaTime, Game &game)
{
	getSprite().setTextureRect(m_animation.getRectangle(deltaTime));
}

void SpiritFeatherItem::draw()
{
	// draw animation with getSprite()
	Renderer::instance().drawAbove(getSprite());
}

Item* SpiritFeatherItem::clone()
{
	return new SpiritFeatherItem(*this);
}