#include "Level.h"
#include "Renderer.h"
#include "Constants.h"

Level::Level()
{
	// Load a default map with nothing but ground tiles
	TileMap::TileMapLayout layout;
	for (int x = 0; x < Constants::MapWidth; x++)
	{
		layout.push_back(std::vector<Tile::TileTypes>());
		for (int y = 0; y < Constants::MapHeight; y++)
			layout[x].push_back(Tile::Ground);
	}

	m_tileMap = MapPtr(new TileMap(layout, Constants::TileSize));
}

void Level::update(float deltaTime)
{

}

void Level::draw(const sf::View &cameraView)
{
	m_tileMap->draw(cameraView);

	for (std::size_t i = 0; i < m_sprites.size(); i++)
	{
		if (m_sprites[i].drawToForeground)
			Renderer::instance().drawForeground(m_sprites[i].sprite);
		else
			Renderer::instance().drawBackground(m_sprites[i].sprite);
	}
}


TileMap& Level::getTileMap()
{
	return *m_tileMap.get();
}
std::vector<LevelSprite>& Level::getDecorations()
{
	return m_sprites;
}