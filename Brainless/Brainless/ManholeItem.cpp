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
	m_examineString = "It reminds Maive of her favorite tv-show. Teenage Mutant Ninja Åke!";

	m_interactDistance = sf::Vector2f(100, 200);

	// Refresh ladder position
	setPosition(getPosition());
	refreshTexture();
}

void ManholeItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);

	writer << m_open << std::endl;
}
void ManholeItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);

	reader >> m_open;
	refreshTexture();
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
		Renderer::instance().drawDepth(getSprite());
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
	{
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("ManholeOpen"));
		m_interactBounds = sf::FloatRect(0, 0,
			getSprite().getGlobalBounds().width,
			getSprite().getGlobalBounds().height + m_ladder.getSprite().getGlobalBounds().height);
	}
	else
	{
		m_interactBounds = sf::FloatRect(0, 0,
			getSprite().getGlobalBounds().width,
			getSprite().getGlobalBounds().height);
		getSprite().setTexture(ResourceLoader::instance().retrieveTexture("ManholeClosed"));
	}
}