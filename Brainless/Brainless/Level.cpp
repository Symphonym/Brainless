#include "Level.h"
#include "Renderer.h"
#include "Constants.h"
#include "Utility.h"

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




#include <iostream>
void Level::updateUnitCollision(float deltaTime)
{
	/*for (unsigned int i = 0; i < m_units.size(); i++)
	{
		Unit* currentUnit = m_units[i].get();
		sf::FloatRect unitBounds = currentUnit->getCollisionRect();
		std::cout << "BOUNDS X: " << unitBounds.left << " BOUNDS Y: " << unitBounds.top << std::endl;
		std::cout << "SIZE X: " << unitBounds.width << " SIZE Y: " << unitBounds.height << std::endl;

		currentUnit->updateMovement(600, deltaTime);

		sf::Vector2i startIndex = m_tileMap->positionToIndex(sf::Vector2f(currentUnit->getPosition().x, currentUnit->getPosition().y));
		startIndex -= sf::Vector2i(1, 1);

		sf::Vector2i endIndex = m_tileMap->positionToIndex(
			sf::Vector2f(currentUnit->getPosition().x+currentUnit->getSize().x, currentUnit->getPosition().y+currentUnit->getSize().y));
		endIndex += sf::Vector2i(1, 1);

		startIndex.x = Utility::clampValue(startIndex.x, 0, Constants::MapWidth - 1);
		startIndex.y = Utility::clampValue(startIndex.y, 0, Constants::MapHeight - 1);

		endIndex.x = Utility::clampValue(endIndex.x, 0, Constants::MapWidth - 1);
		endIndex.y = Utility::clampValue(endIndex.y, 0, Constants::MapHeight - 1);

		std::cout << "STARTINDEX X " << startIndex.x << " STARTINDEX Y " << startIndex.y << std::endl;

		bool collision = false;
		for (int x = startIndex.x; x < endIndex.x; x++)
		{
			for (int y = startIndex.y; y < endIndex.y; y++)
			{
				Tile& currentTile = m_tileMap->getTile(x, y);

				// If tile isn't empty and is colliding with the unit
				if (currentTile.getType() != Tile::Nothing && currentTile.collidesWith(unitBounds))
				{
					collision = true;
					sf::FloatRect tileBounds = currentTile.getSprite().getGlobalBounds();

					// Distances from sides of unit to corresponding sides of tile
					float distanceFromTopToBottom = (tileBounds.top + tileBounds.height) - unitBounds.top;
					float distanceFromBottomToTop = tileBounds.top - (unitBounds.top + unitBounds.height);
					float distanceFromLeftToRight = (tileBounds.left + tileBounds.width) - unitBounds.left;
					float distanceFromRightToLeft = tileBounds.left - (unitBounds.left+unitBounds.width);

					std::cout << "distanceFromTopToBottom: " << distanceFromTopToBottom << std::endl;
					std::cout << "distanceFromBottomToTop: " << distanceFromBottomToTop << std::endl;
					std::cout << "distanceFromLeftToRight: " << distanceFromLeftToRight << std::endl;
					std::cout << "distanceFromRightToLeft: " << distanceFromRightToLeft << std::endl;

					currentUnit->setPosition(sf::Vector2f(currentUnit->getPosition().x, tileBounds.top - unitBounds.height));

					if (std::abs(distanceFromBottomToTop) < Constants::TileSize / 4 && std::abs(distanceFromBottomToTop) > 0)
					{
						currentUnit->setSpeed(sf::Vector2f(currentUnit->getSpeed().x, 0));
						currentUnit->setAcceleration(sf::Vector2f(currentUnit->getAcceleration().x, 0));
						distanceFromBottomToTop = tileBounds.top - (currentUnit->getPosition().y + currentUnit->getSize().y);
						std::cout << "distanceFromBottomToTop: " << distanceFromBottomToTop << std::endl;
						int d = 0;

					}


					//if (std::abs(distanceFromTopToBottom) < Constants::TileSize / 4)
					//	currentUnit->setPosition(currentUnit->getPositionX(), tileBounds.top + tileBounds.height);
					//else if (std::abs(distanceFromBottomToTop) < Constants::TileSize / 4)
					//	currentUnit->setPosition(currentUnit->getPositionX(), tileBounds.top - unitBounds.height);
					//else if(std::abs(distanceFromLeftToRight) < Constants::TileSize / 4)
					//	currentUnit->setPosition(tileBounds.left+tileBounds.width, currentUnit->getPositionY());
					//else if(std::abs(distanceFromRightToLeft) < Constants::TileSize / 4)
					//	currentUnit->setPosition(tileBounds.left, currentUnit->getPositionY());
				}
			}
		}

		if (!collision)
			currentUnit->setStatus(true);
		else
			currentUnit->setStatus(false);

		currentUnit->updateAnimation(deltaTime);
	}*/

	for (unsigned int i = 0; i < m_units.size(); i++)
	{
		//Reset marker color
		//m_markerSprite.setColor(sf::Color::Color(255, 255, 255, 128));

		float old_x = m_units[i]->getPosition().x;
		float old_y = m_units[i]->getPosition().y;
		m_units[i]->updateMovement(600, deltaTime);

		//m_units[i]->updateMovement(600, deltaTime);

		//Find colliding boxes
		int box_start_x = floor(m_units[i]->getPosition().x / Constants::TileSize) - 1;
		int box_end_x = ceil((m_units[i]->getPosition().x + m_units[i]->getSize().x) / Constants::TileSize) + 1;
		int box_start_y = floor(m_units[i]->getPosition().y / Constants::TileSize);
		int box_end_y = ceil((m_units[i]->getPosition().y + m_units[i]->getSize().y) / Constants::TileSize) + 1;
		bool airborne = true;

		for (int x = std::max(box_start_x, 0); x < std::min(box_end_x, Constants::MapWidth); x++)
		{
			for (int y = std::max(box_start_y, 0); y < std::min(box_end_y, Constants::MapHeight); y++)
			{
				if (m_tileMap->getTile(x, y).getType() != Tile::Nothing)
				{
					if (m_tileMap->getTile(x, y).collidesWith(m_units[i]->getCollisionRect()))
					{
						if (std::abs(old_y - (m_units[i]->getPosition().y)) > std::abs(old_x - (m_units[i]->getPosition().x)))
						{
							m_units[i]->setSpeed(sf::Vector2f(m_units[i]->getSpeed().x, 0));
							//units[i]->setAcceleration(units[i]->getAccelerationX(), 0);
						}
						else
						{
							//units[i]->setSpeed(0,units[i]->getSpeedY());
						}
						//Move out of contact
						//units[i]->setPosition(units[i]->getPositionX() - units[i]->getSpeedX(), units[i]->getPositionY() - units[i]->getSpeedY());
						m_units[i]->setPosition(sf::Vector2f(old_x, old_y));
					}

					//Checks if ground is below, makes the player able to jump
					//Kind of a temporary solution, but it works well
					if (m_tileMap->getTile(x, y).collidesWith(sf::FloatRect(m_units[i]->getPosition().x, m_units[i]->getPosition().y + 1, m_units[i]->getSize().x, m_units[i]->getSize().y + 2)))
					{
						airborne = false;
					}

					//Down
					if (m_tileMap->getTile(x, y).collidesWith(sf::FloatRect(m_units[i]->getPosition().x, m_units[i]->getPosition().y, m_units[i]->getSize().x, m_units[i]->getSize().x + 2)))
					{
						m_units[i]->setAcceleration(sf::Vector2f(m_units[i]->getAcceleration().x, 0));
						m_units[i]->setPosition(sf::Vector2f(m_units[i]->getPosition().x, y*Constants::TileSize - m_units[i]->getSize().y));
						m_units[i]->setSpeed(sf::Vector2f(m_units[i]->getSpeed().x, 0));

						airborne = false;
					}
					//Left && Right
					if ((m_units[i]->getSpeed().x<0 && m_tileMap->getTile(x, y).collidesWith(sf::FloatRect(m_units[i]->getPosition().x - 2, m_units[i]->getPosition().y, m_units[i]->getSize().x + 2, m_units[i]->getSize().y))) ||
						(m_units[i]->getSpeed().x>0 && m_tileMap->getTile(x, y).collidesWith(sf::FloatRect(m_units[i]->getPosition().x, m_units[i]->getPosition().y, m_units[i]->getSize().x + 2, m_units[i]->getSize().y))) &&
						airborne == true)
					{
						//m_markerSprite.setColor(sf::Color::Color(128, 64, 0, 128));
						m_units[i]->setSpeed(sf::Vector2f(0, m_units[i]->getSpeed().y));
						m_units[i]->setAcceleration(sf::Vector2f(0, m_units[i]->getAcceleration().y));
						m_units[i]->setPosition(sf::Vector2f(m_units[i]->getPosition().x - m_units[i]->getSpeed().x, m_units[i]->getPosition().y));
					}

				}
			}
		}
		m_units[i]->setStatus(airborne);
		m_units[i]->updateAnimation(deltaTime);

		//Move marker to unit
	}
}