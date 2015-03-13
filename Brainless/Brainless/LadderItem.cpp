#include "ResourceLoader.h"
#include "Utility.h"
#include <string>
#include "LadderItem.h"
#include "Game.h"
#include "Player.h"

LadderItem::LadderItem(const std::string &ladderTexture, int id, bool isFront)
:
Item("Ladder",ladderTexture, ladderTexture, id),
m_inUse(false),
m_reachedStartPos(false),
m_startStartPos(0, 0),
m_startPos(0, 0),
m_endPos(0, 0),
m_lerpValue(0),
m_ladderTexture(ladderTexture)
{
	if (isFront)
		m_renderingMode = RenderingModes::Above;
	else
		m_renderingMode = RenderingModes::Behind;

	m_usable = true;
	m_interactDistance = sf::Vector2f(70, 300);
	
	m_examineString = "To climb or not to climb?";
	m_pickupString = "It's too heavy for me to carry";

	//Change ladder
	m_ladderLenght = -1;
	if ((m_ladderTexture == "WoodLadder1") || (m_ladderTexture == "RopeLadder1") || (m_ladderTexture == "SteelLadder1"))
	{
		m_ladderLenght = 1;
		m_ladderTexture = m_ladderTexture.substr(0, m_ladderTexture.size() - 1);
	}
}


void LadderItem::onUse(Game &game)
{
	if (!m_inUse)
	{
		game.getPlayer().setClimbing(true);
		game.getPlayer().setRenderingMode(Unit::RenderingModes::Above);
		game.getPlayer().setAcceleration(sf::Vector2f(0, 0));
	

		m_inUse = true;
		m_reachedStartPos = false;
		m_startStartPos = game.getPlayer().getPosition();

		// Player is below ladder
		if (game.getPlayer().getPosition().y > getPosition().y)
		{
			m_startPos = sf::Vector2f(
				getSprite().getPosition().x + getSprite().getGlobalBounds().width / 2.f - game.getPlayer().getCollisionRect().width / 2.f,
				game.getPlayer().getPosition().y);
			m_endPos = sf::Vector2f(
				getSprite().getPosition().x + getSprite().getGlobalBounds().width / 2.f - game.getPlayer().getCollisionRect().width / 2.f,
				getSprite().getPosition().y - game.getPlayer().getCollisionRect().height + 5.f); // Add some margin to ground
			game.getPlayer().setSpeed(sf::Vector2f(0, -1));

		}

		// Player is above ladder
		else
		{
			m_startPos = sf::Vector2f(
				getSprite().getPosition().x + getSprite().getGlobalBounds().width / 2.f - game.getPlayer().getCollisionRect().width / 2.f,
				game.getPlayer().getPosition().y);
			m_endPos = sf::Vector2f(
				getSprite().getPosition().x + getSprite().getGlobalBounds().width / 2.f - game.getPlayer().getCollisionRect().width / 2.f,
				getSprite().getPosition().y + getSprite().getGlobalBounds().height - game.getPlayer().getCollisionRect().height - 5.f); // Add some margin to ground
			game.getPlayer().setSpeed(sf::Vector2f(0, 1));

		}

	}
}

void LadderItem::update(float deltaTime, Game &game)
{
	if (m_inUse)
	{
		m_lerpValue += (deltaTime / getSprite().getGlobalBounds().height) * 250.f;//*(1.0f - getSprite().getGlobalBounds().height / 1280.f);

		// Just make sure player is relatively near ladder to begin climbing
		if (!m_reachedStartPos)
		{
			// Speed up the movement to the ladder starting pos
			m_lerpValue += deltaTime * 6.f;

			sf::Vector2f vecToStart = m_startPos - game.getPlayer().getPosition();
			float distanceToStart = std::sqrt(vecToStart.x*vecToStart.x + vecToStart.y*vecToStart.y);

			sf::Vector2f lerpToStart = sf::Vector2f(0, 0);
			lerpToStart.x = m_startStartPos.x + (m_startPos.x - m_startStartPos.x) * m_lerpValue;
			lerpToStart.y = m_startStartPos.y + (m_startPos.y - m_startStartPos.y) * m_lerpValue;

			// Lerp towards start
			game.getPlayer().setPosition(lerpToStart);

			// Player has reached start, so start moving to the end
			if (m_lerpValue >= 1.f)
			{
				m_lerpValue = 0;
				m_reachedStartPos = true;
			}
				
		}
		else
		{
			//sf::Vector2f vecToEnd = m_endPos - game.getPlayer().getPosition();
			//float distanceToEnd = std::sqrt(vecToEnd.x*vecToEnd.x + vecToEnd.y*vecToEnd.y);


			sf::Vector2f lerpToEnd = sf::Vector2f(0, 0);
			lerpToEnd.x = m_startPos.x + (m_endPos.x - m_startPos.x) * m_lerpValue;
			lerpToEnd.y = m_startPos.y + (m_endPos.y - m_startPos.y) * m_lerpValue;

			game.getPlayer().setPosition(lerpToEnd);

			// Player has reached end of ladder
			if (m_lerpValue >= 1.f)
			{
				m_lerpValue = 0;
				m_inUse = false;
				game.getPlayer().setClimbing(false);
				game.getPlayer().setRenderingMode(Unit::RenderingModes::Depth);
				game.getPlayer().setPosition(m_endPos);
				game.getPlayer().setAcceleration(sf::Vector2f(0, 0));
				game.getPlayer().setSpeed(sf::Vector2f(0, 0));
			}
		}
	}
}

void LadderItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);
	writer << m_ladderLenght << m_ladderTexture << std::endl;
}

void LadderItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);
	reader >> m_ladderLenght;
	reader >> m_ladderTexture;
	setLadderTexture(m_ladderLenght, m_ladderTexture);
}

bool LadderItem::onToggle(int direction)
{
	if (m_ladderLenght>0)
	{
		m_ladderLenght = Utility::clampValue<int>(m_ladderLenght + direction,1,10);
		setLadderTexture(m_ladderLenght, m_ladderTexture);
		return true;
	}
	return false;
}

std::string LadderItem::getToggleString() const
{
	return "Ladder size: " + std::to_string(m_ladderLenght);
}

int LadderItem::getLadderLenght()
{
	return m_ladderLenght;
}

std::string LadderItem::getLadderTextureString()
{
	return m_ladderTexture;
}

void LadderItem::setLadderTexture(int ladderLenght,const std::string &ladderTexture)
{
	m_ladderLenght = ladderLenght;
	m_ladderTexture = ladderTexture;
	m_sprite.setTexture(ResourceLoader::instance().retrieveTexture(m_ladderTexture + std::to_string(m_ladderLenght)));
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_sprite.getTexture()->getSize().x, m_sprite.getTexture()->getSize().y));
	m_interactBounds = sf::FloatRect(10,-20,120, 128 * m_ladderLenght + 40);
	m_interactDistance = sf::Vector2f(70, 64 * m_ladderLenght + 120);
}

Item* LadderItem::clone()
{
	return new LadderItem(*this);
}