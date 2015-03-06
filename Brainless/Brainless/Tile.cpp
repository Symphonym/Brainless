#include "Tile.h"
#include "Renderer.h"
#include "ResourceLoader.h"

Tile::Tile(const sf::FloatRect &bounds, TileTypes tileType, const sf::Vector2f &spriteOffset)
:
m_type(tileType),
m_tileSprite(),
m_bounds(bounds),
m_editorVisibleOnly(false),
m_tilt(false),
m_platform(false),
m_autotilingRange("")
{
	updateType();

	m_tileSprite.setPosition(bounds.left, bounds.top);
	m_tileSprite.move(spriteOffset);
	m_tileSprite.setTextureRect(sf::IntRect(0,0,128,128));
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
bool Tile::getEditorVisibleOnly() const
{
	return m_editorVisibleOnly;
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
	m_editorVisibleOnly = false;

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
	case Road_Tilt_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Road_Tilt_Corner"));
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
	
			/*

			WOOD

		*/
	case Wood_Top_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Top_Right")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Top_Right_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Top_Right_Corner")); m_autotilingRange = "WoodAutotiling";
		break;

	case Wood_Top_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Top_Left")); m_autotilingRange = "WoodAutotiling";
		break;
	/*case Wood_Top_Left_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Top_Left_Short")); m_autotilingRange = "WoodAutotiling";
		break;*/
	case Wood_Top_Left_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Top_Left_Corner")); m_autotilingRange = "WoodAutotiling";
		break;

	case Wood_Top:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Top")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Top_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Top_Alone")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Top_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Top_Short")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Top_Corner_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Top_Corner_Left")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Top_Corner_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Top_Corner_Right")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Top_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Top_Corner_Full")); m_autotilingRange = "WoodAutotiling";
		break;


	case Wood_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Right")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Right_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Right_Alone")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Right_Corner_Down:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Right_Corner_Down")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Right_Corner_Top:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Right_Corner_Top")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Right_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Right_Corner_Full")); m_autotilingRange = "WoodAutotiling";
		break;

	case Wood_Middle:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Middle")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Middle_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Middle_Alone")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Middle_Alone_Horizontal:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Middle_Alone_Horizontal")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Middle_Alone_Vertical:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Middle_Alone_Vertical")); m_autotilingRange = "WoodAutotiling";
		break;


	case Wood_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Left")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Left_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Left_Short")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Left_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Left_Alone")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Left_Corner_Down:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Left_Corner_Down")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Left_Corner_Top:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Left_Corner_Top")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Left_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Left_Corner_Full")); m_autotilingRange = "WoodAutotiling";
		break;

	case Wood_Down_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Down_Right")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Down_Right_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Down_Right_Corner")); m_autotilingRange = "WoodAutotiling";
		break;

	case Wood_Down_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Down_Left")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Down_Left_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Down_Left_Corner")); m_autotilingRange = "WoodAutotiling";
		break;

	case Wood_Down:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Down")); m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Down_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Down_Alone"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Down_Corner_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Down_Corner_Left"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Down_Corner_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Down_Corner_Right"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Down_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Down_Corner_Full"));  m_autotilingRange = "WoodAutotiling";
		break;

	case Wood_Tilt:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Tilt"));
		m_tilt = true;
		break;
	case Wood_Tilt_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Tilt_Corner"));
		break;

	case Wood_Corner_TopLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_TopLeft"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Corner_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_TopRight"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Corner_TopLeft_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_TopLeft_TopRight"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Corner_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_DownLeft"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Corner_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_DownRight"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Corner_DownLeft_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_DownLeft_DownRight"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Corner_TopLeft_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_TopLeft_DownLeft"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Corner_TopRight_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_TopRight_DownRight"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Corner_TopLeft_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_TopLeft_DownRight"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Corner_TopRight_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_TopRight_DownLeft"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Corner_Not_TopLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_Not_TopLeft"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Corner_Not_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_Not_TopRight"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Corner_Not_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_Not_DownLeft"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Corner_Not_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_Not_DownRight"));  m_autotilingRange = "WoodAutotiling";
		break;
	case Wood_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Wood_Corner_Full"));  m_autotilingRange = "WoodAutotiling";
		break;

		/*

		Grass

		*/
	case Grass_Top_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Top_Right")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Top_Right_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Top_Right_Corner")); m_autotilingRange = "GrassAutotiling";
		break;

	case Grass_Top_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Top_Left")); m_autotilingRange = "GrassAutotiling";
		break;
		/*case Grass_Top_Left_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Top_Left_Short")); m_autotilingRange = "GrassAutotiling";
		break;*/
	case Grass_Top_Left_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Top_Left_Corner")); m_autotilingRange = "GrassAutotiling";
		break;

	case Grass_Top:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Top")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Top_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Top_Alone")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Top_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Top_Short")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Top_Corner_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Top_Corner_Left")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Top_Corner_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Top_Corner_Right")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Top_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Top_Corner_Full")); m_autotilingRange = "GrassAutotiling";
		break;


	case Grass_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Right")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Right_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Right_Alone")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Right_Corner_Down:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Right_Corner_Down")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Right_Corner_Top:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Right_Corner_Top")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Right_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Right_Corner_Full")); m_autotilingRange = "GrassAutotiling";
		break;

	case Grass_Middle:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Middle")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Middle_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Middle_Alone")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Middle_Alone_Horizontal:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Middle_Alone_Horizontal")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Middle_Alone_Vertical:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Middle_Alone_Vertical")); m_autotilingRange = "GrassAutotiling";
		break;


	case Grass_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Left")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Left_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Left_Short")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Left_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Left_Alone")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Left_Corner_Down:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Left_Corner_Down")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Left_Corner_Top:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Left_Corner_Top")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Left_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Left_Corner_Full")); m_autotilingRange = "GrassAutotiling";
		break;

	case Grass_Down_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Down_Right")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Down_Right_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Down_Right_Corner")); m_autotilingRange = "GrassAutotiling";
		break;

	case Grass_Down_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Down_Left")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Down_Left_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Down_Left_Corner")); m_autotilingRange = "GrassAutotiling";
		break;

	case Grass_Down:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Down")); m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Down_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Down_Alone"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Down_Corner_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Down_Corner_Left"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Down_Corner_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Down_Corner_Right"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Down_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Down_Corner_Full"));  m_autotilingRange = "GrassAutotiling";
		break;

	case Grass_Tilt:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Tilt"));
		m_tilt = true;
		break;
	case Grass_Tilt_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Tilt_Corner"));
		break;

	case Grass_Corner_TopLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_TopLeft"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Corner_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_TopRight"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Corner_TopLeft_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_TopLeft_TopRight"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Corner_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_DownLeft"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Corner_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_DownRight"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Corner_DownLeft_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_DownLeft_DownRight"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Corner_TopLeft_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_TopLeft_DownLeft"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Corner_TopRight_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_TopRight_DownRight"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Corner_TopLeft_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_TopLeft_DownRight"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Corner_TopRight_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_TopRight_DownLeft"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Corner_Not_TopLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_Not_TopLeft"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Corner_Not_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_Not_TopRight"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Corner_Not_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_Not_DownLeft"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Corner_Not_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_Not_DownRight"));  m_autotilingRange = "GrassAutotiling";
		break;
	case Grass_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Grass_Corner_Full"));  m_autotilingRange = "GrassAutotiling";
		break;

		/*

		Sewer

		*/
	case Sewer_Top_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Top_Right")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Top_Right_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Top_Right_Corner")); m_autotilingRange = "SewerAutotiling";
		break;

	case Sewer_Top_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Top_Left")); m_autotilingRange = "SewerAutotiling";
		break;
		/*case Sewer_Top_Left_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Top_Left_Short")); m_autotilingRange = "SewerAutotiling";
		break;*/
	case Sewer_Top_Left_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Top_Left_Corner")); m_autotilingRange = "SewerAutotiling";
		break;

	case Sewer_Top:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Top")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Top_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Top_Alone")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Top_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Top_Short")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Top_Corner_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Top_Corner_Left")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Top_Corner_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Top_Corner_Right")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Top_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Top_Corner_Full")); m_autotilingRange = "SewerAutotiling";
		break;


	case Sewer_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Right")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Right_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Right_Alone")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Right_Corner_Down:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Right_Corner_Down")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Right_Corner_Top:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Right_Corner_Top")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Right_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Right_Corner_Full")); m_autotilingRange = "SewerAutotiling";
		break;

	case Sewer_Middle:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Middle")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Middle_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Middle_Alone")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Middle_Alone_Horizontal:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Middle_Alone_Horizontal")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Middle_Alone_Vertical:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Middle_Alone_Vertical")); m_autotilingRange = "SewerAutotiling";
		break;


	case Sewer_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Left")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Left_Short:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Left_Short")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Left_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Left_Alone")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Left_Corner_Down:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Left_Corner_Down")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Left_Corner_Top:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Left_Corner_Top")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Left_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Left_Corner_Full")); m_autotilingRange = "SewerAutotiling";
		break;

	case Sewer_Down_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Down_Right")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Down_Right_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Down_Right_Corner")); m_autotilingRange = "SewerAutotiling";
		break;

	case Sewer_Down_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Down_Left")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Down_Left_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Down_Left_Corner")); m_autotilingRange = "SewerAutotiling";
		break;

	case Sewer_Down:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Down")); m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Down_Alone:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Down_Alone"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Down_Corner_Left:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Down_Corner_Left"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Down_Corner_Right:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Down_Corner_Right"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Down_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Down_Corner_Full"));  m_autotilingRange = "SewerAutotiling";
		break;

	case Sewer_Tilt:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Tilt"));
		m_tilt = true;
		break;
	case Sewer_Tilt_Corner:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Tilt_Corner"));
		break;

	case Sewer_Corner_TopLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_TopLeft"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Corner_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_TopRight"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Corner_TopLeft_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_TopLeft_TopRight"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Corner_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_DownLeft"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Corner_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_DownRight"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Corner_DownLeft_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_DownLeft_DownRight"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Corner_TopLeft_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_TopLeft_DownLeft"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Corner_TopRight_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_TopRight_DownRight"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Corner_TopLeft_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_TopLeft_DownRight"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Corner_TopRight_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_TopRight_DownLeft"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Corner_Not_TopLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_Not_TopLeft"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Corner_Not_TopRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_Not_TopRight"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Corner_Not_DownLeft:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_Not_DownLeft"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Corner_Not_DownRight:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_Not_DownRight"));  m_autotilingRange = "SewerAutotiling";
		break;
	case Sewer_Corner_Full:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("Sewer_Corner_Full"));  m_autotilingRange = "SewerAutotiling";
		break;

	case Solid_Invisible:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("SolidInvisibleCube"));
		m_editorVisibleOnly = true;
		break;
	case Solid_Invisible_Platform:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("SolidInvisiblePlatformCube"));
		m_editorVisibleOnly = true;
		m_platform = true;
		break;
		
	default:
		break;
	}
}