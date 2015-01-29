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

Unit* Level::addUnit(UnitPtr unit)
{
	m_units.push_back(std::move(unit));
	return m_units.back().get();
}

void Level::update(float deltaTime)
{
	const int unitSpeed = 600;
	for (std::size_t i = 0; i < m_units.size(); i++)
	{
		// TODO update units
		
	}

	updateUnitCollision(deltaTime);
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

	for (std::size_t i = 0; i < m_items.size(); i++)
		m_items[i]->draw();
	for (std::size_t i = 0; i < m_units.size(); i++)
		m_units[i]->draw();
}

TileMap& Level::getTileMap()
{
	return *m_tileMap.get();
}
std::vector<Level::ItemPtr>& Level::getItems()
{
	return m_items;
}
std::vector<LevelSprite>& Level::getDecorations()
{
	return m_sprites;
}
std::vector<Level::UnitPtr>& Level::getUnits()
{
	return m_units;
}






void Level::updateUnitCollision(float deltaTime)
{
	for (unsigned int i = 0; i < m_units.size(); i++)
	{
		//Reset marker color
		//m_markerSprite.setColor(sf::Color::Color(255, 255, 255, 128));

		float old_x = m_units[i]->getPositionX();
		float old_y = m_units[i]->getPositionY();
		m_units[i]->updateMovement(600, deltaTime);

		//m_units[i]->updateMovement(600, deltaTime);

		//Find colliding boxes
		int box_start_x = floor(m_units[i]->getPositionX() / Constants::TileSize) - 1;
		int box_end_x = ceil((m_units[i]->getPositionX() + m_units[i]->getWidth()) / Constants::TileSize) + 1;
		int box_start_y = floor(m_units[i]->getPositionY() / Constants::TileSize);
		int box_end_y = ceil((m_units[i]->getPositionY() + m_units[i]->getHeight()) / Constants::TileSize) + 1;
		bool airborne = true;

		for (int x = std::max(box_start_x, 0); x < std::min(box_end_x, Constants::MapWidth); x++)
		{
			for (int y = std::max(box_start_y, 0); y < std::min(box_end_y, Constants::MapHeight); y++)
			{
				if (m_tileMap->getTile(x, y).getType() != Tile::Nothing)
				{
					if (m_tileMap->getTile(x, y).collidesWith(m_units[i]->getCollisionRect()))
					{
						if (std::abs(old_y - (m_units[i]->getPositionY())) > std::abs(old_x - (m_units[i]->getPositionX())))
						{
							m_units[i]->setSpeed(m_units[i]->getSpeedX(), 0);
							//units[i]->setAcceleration(units[i]->getAccelerationX(), 0);
						}
						else
						{
							//units[i]->setSpeed(0,units[i]->getSpeedY());
						}
						//Move out of contact
						//units[i]->setPosition(units[i]->getPositionX() - units[i]->getSpeedX(), units[i]->getPositionY() - units[i]->getSpeedY());
						m_units[i]->setPosition(old_x, old_y);
					}

					//Checks if ground is below, makes the player able to jump
					//Kind of a temporary solution, but it works well
					if (m_tileMap->getTile(x, y).collidesWith(sf::FloatRect(m_units[i]->getPositionX(), m_units[i]->getPositionY() + 1, m_units[i]->getWidth(), m_units[i]->getHeight() + 2)))
					{
						airborne = false;
					}

					//Down
					if (m_tileMap->getTile(x, y).collidesWith(sf::FloatRect(m_units[i]->getPositionX(), m_units[i]->getPositionY(), m_units[i]->getWidth(), m_units[i]->getHeight() + 2)))
					{
						m_units[i]->setAcceleration(m_units[i]->getAccelerationX(), 0);
						m_units[i]->setPosition(m_units[i]->getPositionX(), y*Constants::TileSize - m_units[i]->getHeight());
						m_units[i]->setSpeed(m_units[i]->getSpeedX(), 0);

						airborne = false;
					}
					//Left && Right
					if ((m_units[i]->getSpeedX()<0 && m_tileMap->getTile(x, y).collidesWith(sf::FloatRect(m_units[i]->getPositionX() - 2, m_units[i]->getPositionY(), m_units[i]->getWidth() + 2, m_units[i]->getHeight()))) ||
						(m_units[i]->getSpeedX()>0 && m_tileMap->getTile(x, y).collidesWith(sf::FloatRect(m_units[i]->getPositionX(), m_units[i]->getPositionY(), m_units[i]->getWidth() + 2, m_units[i]->getHeight()))) &&
						airborne == true)
					{
						//m_markerSprite.setColor(sf::Color::Color(128, 64, 0, 128));
						m_units[i]->setSpeed(0, m_units[i]->getSpeedY());
						m_units[i]->setAcceleration(0, m_units[i]->getAccelerationY());
						m_units[i]->setPosition(m_units[i]->getPositionX() - m_units[i]->getSpeedX(), m_units[i]->getPositionY());
					}

				}
			}
		}
		m_units[i]->setStatus(airborne);
		m_units[i]->updateAnimation(deltaTime);

		//Move marker to unit
	}
}