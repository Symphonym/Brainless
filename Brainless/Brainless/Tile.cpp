#include "Tile.h"
#include "Renderer.h"
#include "ResourceLoader.h"

Tile::Tile(const sf::FloatRect &bounds, TileTypes tileType, const sf::Vector2f &spriteOffset)
:
m_type(tileType),
m_tileSprite(),
m_bounds(bounds)
{
	ResourceLoader::instance().loadTexture("GroundCube", "cube.png");
	ResourceLoader::instance().loadTexture("RedCube", "cubeRed.png");
	ResourceLoader::instance().loadTexture("BlueCube", "cubeBlue.png");
	ResourceLoader::instance().loadTexture("AirCube", "cubeNada.png");

	updateType();

	// sprite.setTexture(ResourceLoader......);
	
	m_tileSprite.setPosition(bounds.left, bounds.top);
	m_tileSprite.move(spriteOffset);
}

void Tile::setType(TileTypes type)
{
	m_type = type;
	updateType();
}

void Tile::draw(bool debug)
{
	Renderer::instance().draw(m_tileSprite, m_tileSprite);

	if (debug)
	{
		sf::Image colImg;
		colImg.create(m_bounds.width, m_bounds.height, sf::Color::Green);

		sf::Texture tex; tex.loadFromImage(colImg);
		sf::Sprite spr;
		spr.setTexture(tex);
		spr.setPosition(m_bounds.left, m_bounds.top);

		Renderer::instance().draw(spr, spr);
	}
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



void Tile::updateType()
{
	switch (m_type)
	{
	case Nothing:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("AirCube"));
		break;
	case Ground:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("GroundCube"));
		break;
	case Red:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("RedCube"));
		break;
	case Blue:
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("BlueCube"));
		break;
		// TODO Load texture based on type
	}
}