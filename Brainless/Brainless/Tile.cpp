#include "Tile.h"
#include "Renderer.h"

Tile::Tile(const sf::FloatRect &bounds, TileTypes tileType, const sf::Vector2f &spriteOffset)
:
m_type(tileType),
m_tileSprite(),
m_bounds(bounds)
{
	switch (tileType)
	{
		// TODO Load texture based on type
	}

	// sprite.setTexture(ResourceLoader......);
	m_tileSprite.move(spriteOffset);
}


void Tile::draw(const sf::RenderTarget &target)
{
	Renderer::instance().draw(m_tileSprite);
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