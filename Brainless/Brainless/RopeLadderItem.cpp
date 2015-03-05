#include "RopeLadderItem.h"
#include "Renderer.h"
#include "ResourceLoader.h"

RopeLadderItem::RopeLadderItem(const std::string &ladderTexture, int id)
:
Item("Rope Ladder", "RopeLadderCompact", "RopeLadderCompact", id),
m_ladder(ladderTexture, id),
m_ladderIsDown(false)
{
	m_lootable = false;
	refreshStatus();
	m_useString = "I can't reach that from here";
	m_interactDistance = sf::Vector2f(700, 1000);
	//m_interactDistance = sf::Vector2f(100, 1000);
}

void RopeLadderItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);

	writer << m_ladderIsDown << std::endl;
}
void RopeLadderItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);

	reader >> m_ladderIsDown;
	refreshStatus();
}

bool RopeLadderItem::onInteractedWith(Item &otherItem, Game &game)
{
	if (otherItem.getName() == "Key & String" && !m_ladderIsDown)
	{
		m_ladderIsDown = true;
		refreshStatus();
	}

	return false;
}

Item* RopeLadderItem::clone()
{
	return new RopeLadderItem(*this);
}

void RopeLadderItem::onUse(Game &game)
{
	if (!m_ladderIsDown)
	{
		m_ladderIsDown = true;
		refreshStatus();
	}
	else
	{
		m_ladder.onUse(game);
	}

}
void RopeLadderItem::onPositionChanged()
{
	m_ladder.setPosition(
		sf::Vector2f(getPosition().x + getSprite().getGlobalBounds().width / 2.f - m_ladder.getSprite().getGlobalBounds().width / 2.f, getSprite().getPosition().y));
}

void RopeLadderItem::update(float deltaTime, Game &game)
{
	m_ladder.update(deltaTime, game);
}

void RopeLadderItem::draw()
{
	if (m_ladderIsDown)
	{
		Renderer::instance().drawDepth(getSprite());
		Renderer::instance().drawAbove(m_ladder.getSprite());
	}
	else
	{
		Renderer::instance().drawDepth(getSprite());
	}
}

void RopeLadderItem::refreshStatus()
{
	if (m_ladderIsDown)
	{
		m_interactDistance = sf::Vector2f(100, 1000);
		m_examineString = "It's a rope ladder hanging down, I could try climbing it";
		m_usable = true;
		//getSprite().setTexture(ResourceLoader::instance().retrieveTexture("ManholeOpen"));
		m_interactBounds = sf::FloatRect(0, 0,
			getSprite().getGlobalBounds().width,
			getSprite().getGlobalBounds().height + m_ladder.getSprite().getGlobalBounds().height);
	}
	else
	{
		m_interactDistance = sf::Vector2f(700, 1000);
		m_examineString = "It's a rope ladder all curled up, I should find a way to get it to fall down";
		m_usable = false;
		m_interactBounds = sf::FloatRect(0, 0,
			getSprite().getGlobalBounds().width,
			getSprite().getGlobalBounds().height);
		//getSprite().setTexture(ResourceLoader::instance().retrieveTexture("ManholeClosed"));
	}
}