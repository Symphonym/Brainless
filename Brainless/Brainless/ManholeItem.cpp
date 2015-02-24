#include "ManholeItem.h"
#include "ResourceLoader.h"
#include "Renderer.h"

ManholeItem::ManholeItem(bool isOpen, const std::string &ladderTexture, int id)
:
Item("Manhole", "ManholeClosed", "ManholeClosed", id),
m_open(isOpen),
m_ladder(ladderTexture, id)
{
	m_lootable = false;
	m_usable = true;
	m_solid = true;
	m_collidable = true;
	m_examineString = "It reminds Maive of her favorite tv-show. Teenage Mutant Ninja �ke!";

	m_collisionBounds = sf::FloatRect(
		0, 0,
		getSprite().getGlobalBounds().width,
		getSprite().getGlobalBounds().height);
	m_interactBounds = sf::FloatRect(0, 0,
		getSprite().getGlobalBounds().width,
		getSprite().getGlobalBounds().height + m_ladder.getSprite().getGlobalBounds().height);

	// Refresh ladder position
	setPosition(getPosition());
}

void ManholeItem::onUse(Game &game)
{
	if (!m_open)
	{
		m_open = true;
		refreshTexture();
	}
	else
	{
		m_ladder.onUse(game);
	}

}
void ManholeItem::onPositionChanged()
{
	m_ladder.setPosition(
		sf::Vector2f(getPosition().x + getSprite().getGlobalBounds().width / 2.f - m_ladder.getSprite().getGlobalBounds().width / 2.f, getSprite().getPosition().y));
}

void ManholeItem::update(float deltaTime, Game &game)
{
	m_ladder.update(deltaTime, game);
}

void ManholeItem::draw()
{
	if (m_open)
	{
		Renderer::instance().drawBehind(getSprite());
		Renderer::instance().drawAbove(m_ladder.getSprite());
	}
	else
	{
		Renderer::instance().drawDepth(getSprite());
	}
}

Item* ManholeItem::clone()
{
	return new ManholeItem(*this);
}

void ManholeItem::refreshTexture()
{
	if (m_open)
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("ManholeOpen"));
	else
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("ManholeClosed"));
}