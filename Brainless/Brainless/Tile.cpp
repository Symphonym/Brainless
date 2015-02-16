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
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Top-Right"));
		break;
	case Road_Top_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Top-Left"));
		break;
	case Road_Top_Left_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Top-Left-Short"));
		break;
	case Road_Tilt:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Tilt"));
		m_tilt = true;
		break;
	case Road_Top:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Top"));
		break;
	case Road_Top_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Top-Alone"));
		break;
	case Road_Top_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Top-Short"));
		break;
	case Road_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Right"));
		break;
	case Road_Right_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Right-Alone"));
		break;
	case Road_Middle:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Middle"));
		break;
	case Road_Middle_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Middle-Alone"));
		break;
	case Road_Middle_Alone_Horizontal:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Middle-Alone-Horizontal"));
		break;
	case Road_Middle_Alone_Vertical:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Middle-Alone-Vertical"));
		break;
	case Road_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Left"));
		break;
	case Road_Left_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Left-Short"));
		break;
	case Road_Left_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Left-Alone"));
		break;
	case Road_Down_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Down-Right"));
		break;
	case Road_Down_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Down-Left"));
		break;
	case Road_Down:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Down"));
		break;
	case Road_Down_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Down-Alone"));
		break;
	case Road_Corner_TopLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-TopLeft"));
		break;
	case Road_Corner_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-TopRight"));
		break;
	case Road_Corner_TopLeft_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-TopLeft-TopRight"));
		break;
	case Road_Corner_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-DownLeft"));
		break;
	case Road_Corner_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-DownRight"));
		break;
	case Road_Corner_DownLeft_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-DownLeft-DownRight"));
		break;
	case Road_Corner_TopLeft_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-TopLeft-DownLeft"));
		break;
	case Road_Corner_TopRight_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-TopRight-DownRight"));
		break;
	case Road_Corner_TopLeft_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-TopLeft-DownRight"));
		break;
	case Road_Corner_TopRight_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-TopRight-DownLeft"));
		break;
	case Road_Corner_Not_TopLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-Not-TopLeft"));
		break;
	case Road_Corner_Not_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-Not-TopRight"));
		break;
	case Road_Corner_Not_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-Not-DownLeft"));
		break;
	case Road_Corner_Not_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-Not-DownRight"));
		break;
	case Road_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-Full"));
		break;
	
	default:
		break;
	}
}