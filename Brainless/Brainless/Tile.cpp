#include "Tile.h"
#include "Renderer.h"
#include "ResourceLoader.h"

Tile::Tile(const sf::FloatRect &bounds, TileTypes tileType, const sf::Vector2f &spriteOffset)
:
m_type(tileType),
m_tileSprite(),
m_bounds(bounds),
m_tilt(false),
m_platform(false)
{
	updateType();

	m_tileSprite.setPosition(bounds.left, bounds.top);
	m_tileSprite.move(spriteOffset);
}

void Tile::setType(TileTypes type)
{
	m_type = type;
	updateType();
}

void Tile::draw()
{
	//Renderer::instance().draw(m_tileSprite, m_tileSprite);
	Renderer::instance().drawDepth(m_tileSprite);
}

// Checks if the tile collides with another bounding box
bool Tile::collidesWith(const sf::FloatRect &rect) const
{
	return m_bounds.intersects(rect);
}
const sf::FloatRect& Tile::getBounds() const
{
	return m_bounds;
}
Tile::TileTypes Tile::getType() const
{
	return m_type;
}
sf::Sprite& Tile::getSprite()
{
	return m_tileSprite;
}

bool Tile::getTilt() const
{
	return m_tilt;
}

bool Tile::getPlatform() const
{
	return m_platform;
}

void Tile::updateType()
{
	switch (m_type)
	{
	case Nothing:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("AirCube"));
		break;
	case Ground:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("SmoothCube"));
		break;
	case Red:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("StandardCube"));
		break;
	case Blue:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("BlueCube"));
		break;
	case Tilt:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("TiltCube"));
		m_tilt = true;
		break;
	case Platform:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("PlatformCube"));
		m_platform = true;
		break;

	case Road_Top_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R_Top_Right"));
		m_platform = true;
		break;
	default:
		break;
		// TODO Load texture based on type
	}
}