#include "Tile.h"
#include "Renderer.h"
#include "ResourceLoader.h"

Tile::Tile(const sf::FloatRect &bounds, TileTypes tileType, const sf::Vector2f &spriteOffset)
:
m_type(tileType),
m_tileSprite(),
m_bounds(bounds),
m_tilt(false),
m_platform(false),
m_autotilingRange("")
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
std::string Tile::getAutotilingRangeName() const
{
	return m_autotilingRange;
}

void Tile::updateType()
{
	m_autotilingRange = "";

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
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Top-Right")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Top_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Top-Left")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Top_Left_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Top-Left-Short")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Tilt:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Tilt"));
		m_tilt = true;
		break;
	case Road_Top:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Top")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Top_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Top-Alone")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Top_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Top-Short")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Right")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Right_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Right-Alone")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Middle:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Middle")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Middle_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Middle-Alone")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Middle_Alone_Horizontal:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Middle-Alone-Horizontal")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Middle_Alone_Vertical:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Middle-Alone")); m_autotilingRange = "RoadAutotiling"; //SAKNAS R-Middle-Alone-Vertical
		break;
	case Road_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Left")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Left_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Left-Short")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Left_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Left-Alone")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Down_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Down-Right")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Down_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Down-Left")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Down:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Down")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-Left")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("R-Corner-Right")); m_autotilingRange = "RoadAutotiling";
		break;

	default:
		break;
	}
}