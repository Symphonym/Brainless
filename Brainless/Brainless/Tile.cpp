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
	m_tilt = false;
	m_platform = false;

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
		/*

			ROAD

		*/
	case Road_Top_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Top_Right")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Top_Right_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Top_Right_Corner")); m_autotilingRange = "RoadAutotiling";
		break;

	case Road_Top_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Top_Left")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Top_Left_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Top_Left_Short")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Top_Left_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Top_Left_Corner")); m_autotilingRange = "RoadAutotiling";
		break;

	case Road_Top:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Top")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Top_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Top_Alone")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Top_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Top_Short")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Top_Corner_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Top_Corner_Left")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Top_Corner_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Top_Corner_Right")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Top_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Top_Corner_Full")); m_autotilingRange = "RoadAutotiling";
		break;


	case Road_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Right")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Right_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Right_Alone")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Right_Corner_Down:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Right_Corner_Down")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Right_Corner_Top:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Right_Corner_Top")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Right_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Right_Corner_Full")); m_autotilingRange = "RoadAutotiling";
		break;

	case Road_Middle:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Middle")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Middle_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Middle_Alone")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Middle_Alone_Horizontal:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Middle_Alone_Horizontal")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Middle_Alone_Vertical:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Middle_Alone_Vertical")); m_autotilingRange = "RoadAutotiling";
		break;


	case Road_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Left")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Left_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Left_Short")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Left_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Left_Alone")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Left_Corner_Down:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Left_Corner_Down")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Left_Corner_Top:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Left_Corner_Top")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Left_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Left_Corner_Full")); m_autotilingRange = "RoadAutotiling";
		break;

	case Road_Down_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Down_Right")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Down_Right_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Down_Right_Corner")); m_autotilingRange = "RoadAutotiling";
		break;

	case Road_Down_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Down_Left")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Down_Left_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Down_Left_Corner")); m_autotilingRange = "RoadAutotiling";
		break;

	case Road_Down:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Down")); m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Down_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Down_Alone"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Down_Corner_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Down_Corner_Left"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Down_Corner_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Down_Corner_Right"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Down_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Down_Corner_Full"));  m_autotilingRange = "RoadAutotiling";
		break;

	case Road_Tilt:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Tilt"));
		m_tilt = true;
		break;

	case Road_Corner_TopLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_TopLeft"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_TopRight"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_TopLeft_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_TopLeft_TopRight"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_DownLeft"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_DownRight"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_DownLeft_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_DownLeft_DownRight"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_TopLeft_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_TopLeft_DownLeft"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_TopRight_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_TopRight_DownRight"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_TopLeft_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_TopLeft_DownRight"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_TopRight_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_TopRight_DownLeft"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_Not_TopLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_Not_TopLeft"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_Not_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_Not_TopRight"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_Not_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_Not_DownLeft"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_Not_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_Not_DownRight"));  m_autotilingRange = "RoadAutotiling";
		break;
	case Road_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Corner_Full"));  m_autotilingRange = "RoadAutotiling";
		break;

	default:
		break;
	}
}