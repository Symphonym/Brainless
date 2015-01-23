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

	switch (tileType)
	{
	case Ground: 
		m_tileSprite.setTexture(ResourceLoader::instance().retrieveTexture("GroundCube"));
		//m_tileSprite.setScale(
		//	m_bounds.width / m_tileSprite.getTexture()->getSize().x,
		//	m_bounds.height / m_tileSprite.getTexture()->getSize().y);
		break;
		// TODO Load texture based on type
	}

	// sprite.setTexture(ResourceLoader......);
	
	m_tileSprite.setPosition(bounds.left, bounds.top);
	m_tileSprite.move(spriteOffset);
}


void Tile::draw(bool debug)
{
	if (debug)
	{
		
	}


	Renderer::instance().draw(m_tileSprite);

	if (debug)
	{
		sf::Image colImg;
		colImg.create(m_bounds.width, m_bounds.height, sf::Color::Green);

		sf::Texture tex; tex.loadFromImage(colImg);
		sf::Sprite spr;
		spr.setTexture(tex);
		spr.setPosition(m_bounds.left, m_bounds.top);

		Renderer::instance().draw(spr);
	}
}

// Checks if the tile collides with another bounding box
bool Tile::collidesWith(const sf::FloatRect &rect)
{
	return m_bounds.intersects(rect);
}
const sf::FloatRect& Tile::getBounds() const
{
	return m_bounds;
}