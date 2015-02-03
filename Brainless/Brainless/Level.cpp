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

void Level::addItem(ItemPtr item)
{
	m_items.push_back(std::move(item));
}
void Level::addDecoration(const LevelSprite &decoration)
{
	m_sprites.push_back(decoration);
}

Level::ItemPtr Level::removeItem(Item *item)
{
	ItemPtr itemCopy;
	for (std::size_t i = 0; i < m_items.size(); i++)
	{
		if (m_items[i].get() == item)
		{
			itemCopy = std::move(m_items[i]);
			m_items.erase(m_items.begin() + i);
		}
	}

	return std::move(itemCopy);
}
Level::ItemPtr Level::removeItem(std::size_t index)
{
	ItemPtr itemCopy;
	if (index < m_items.size())
	{
		itemCopy = std::move(m_items[index]);
		m_items.erase(m_items.begin() + index);
	}
	return std::move(itemCopy);
}
void Level::removeDecoration(std::size_t index)
{
	if (index < m_sprites.size())
		m_sprites.erase(m_sprites.begin() + index);
}


void Level::reset()
{
	for (int x = 0; x < Constants::MapWidth; x++)
	{
		for (int y = 0; y < Constants::MapHeight; y++)
			m_tileMap->getTile(x, y).setType(Tile::Ground);
	}

	m_units.clear();
	m_items.clear();
	m_sprites.clear();
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
const std::vector<Level::ItemPtr>& Level::getItems() const
{
	return m_items;
}
const std::vector<LevelSprite>& Level::getDecorations() const
{
	return m_sprites;
}
const std::vector<Level::UnitPtr>& Level::getUnits() const
{
	return m_units;
}

Item& Level::getItem(std::size_t index)
{
	return *m_items.at(index).get();
}
Unit& Level::getUnit(std::size_t index)
{
	return *m_units.at(index).get();
}




#include <iostream>
void Level::updateUnitCollision(float deltaTime)
{
	for (unsigned int i = 0; i < m_units.size(); i++)
	{
		Unit* currentUnit = m_units[i].get();
		sf::FloatRect unitBounds = currentUnit->getCollisionRect();
		//std::cout << "BOUNDS X: " << unitBounds.left << " BOUNDS Y: " << unitBounds.top << std::endl;
		//std::cout << "SIZE X: " << unitBounds.width << " SIZE Y: " << unitBounds.height << std::endl;

		currentUnit->updateTask(deltaTime);
		currentUnit->updateMovement(Constants::Gravity, deltaTime);

		sf::Vector2i startIndex = m_tileMap->positionToIndex(sf::Vector2f(currentUnit->getPosition().x, currentUnit->getPosition().y));
		startIndex -= sf::Vector2i(1, 1);

		sf::Vector2i endIndex = m_tileMap->positionToIndex(
			sf::Vector2f(currentUnit->getPosition().x+currentUnit->getSize().x, currentUnit->getPosition().y+currentUnit->getSize().y));
		endIndex += sf::Vector2i(1, 1);

		startIndex.x = Utility::clampValue(startIndex.x, 0, Constants::MapWidth - 1);
		startIndex.y = Utility::clampValue(startIndex.y, 0, Constants::MapHeight - 1);

		endIndex.x = Utility::clampValue(endIndex.x, 0, Constants::MapWidth - 1);
		endIndex.y = Utility::clampValue(endIndex.y, 0, Constants::MapHeight - 1);

		//std::cout << "STARTINDEX X " << startIndex.x << " STARTINDEX Y " << startIndex.y << " " << currentUnit->getInAir() << std::endl;

		currentUnit->setStatus(true);

		bool collision = false;
		bool onGround = false;
		bool leftEdge = false;
		bool rightEdge = false;
		float glideSpeed = 80;

		sf::FloatRect testRectLeft = sf::FloatRect(unitBounds.left + 1, unitBounds.top + unitBounds.height, 1, 1);
		sf::FloatRect testRectRight = sf::FloatRect(unitBounds.left + unitBounds.width - 1, unitBounds.top + unitBounds.height, 1, 1);

		for (int x = startIndex.x; x < endIndex.x; x++)
		{
			for (int y = startIndex.y; y < endIndex.y; y++)
			{
				Tile& currentTile = m_tileMap->getTile(x, y);

				if (currentTile.getType() != Tile::Nothing && currentTile.collidesWith(sf::FloatRect(unitBounds.left + 20, unitBounds.top + unitBounds.height, unitBounds.width - 40, 1)))
				{
						currentUnit->setStatus(false);	
						onGround = true;
				}

				if (currentTile.getType() != Tile::Nothing && currentTile.collidesWith(testRectLeft))
				{
					leftEdge = true;
				}

				if (currentTile.getType() != Tile::Nothing && currentTile.collidesWith(testRectRight))
				{
					rightEdge = true;
				}

				bool hasCollided = false;
				sf::FloatRect originalBounds = unitBounds;

				for (int i = 0; i < 2; i++)
				{
					if (i == 0)
					{
						unitBounds.top = unitBounds.top + unitBounds.height - unitBounds.width;
						unitBounds.height = unitBounds.width;
					}
					else
					{
						unitBounds.top = originalBounds.top;
					}

					// If tile isn't empty and is colliding with the unit
					if (currentTile.getType() != Tile::Nothing && currentTile.collidesWith(unitBounds) && !hasCollided)
					{
						collision = true;
						hasCollided = true;
						sf::FloatRect tileBounds = currentTile.getBounds();
						sf::Vector2f tileCenter = sf::Vector2f(tileBounds.left + tileBounds.width / 2, tileBounds.top + tileBounds.height / 2);
						sf::Vector2f unitCenter = sf::Vector2f(unitBounds.left + unitBounds.width / 2, unitBounds.top + unitBounds.height / 2);

						//Kolla om kollisionen är vertikal
						if (abs(unitCenter.x - tileCenter.x) < abs(unitCenter.y - tileCenter.y))
						{
							//Kolla om unit är ovanför tile
							if (unitCenter.y < tileCenter.y)
							{
								currentUnit->setSpeed(sf::Vector2f(currentUnit->getSpeed().x, 0));
								currentUnit->setAcceleration(sf::Vector2f(currentUnit->getAcceleration().x, 0));
								currentUnit->setPosition(sf::Vector2f(currentUnit->getPosition().x, tileBounds.top - originalBounds.height));
								std::cout << "above" << std::endl;
							}
							else if (unitCenter.y > tileCenter.y)
							{
								currentUnit->setSpeed(sf::Vector2f(currentUnit->getSpeed().x, 0));
								currentUnit->setAcceleration(sf::Vector2f(currentUnit->getAcceleration().x, 0));
								currentUnit->setPosition(sf::Vector2f(currentUnit->getPosition().x, tileBounds.top + tileBounds.height));
								std::cout << "under" << std::endl;
							}
						}
						else if (abs(unitCenter.x - tileCenter.x) > abs(unitCenter.y - tileCenter.y))
						{
							//unit vänster om tile
							if (unitCenter.x < tileCenter.x)
							{
								currentUnit->setSpeed(sf::Vector2f(0, currentUnit->getSpeed().y));
								currentUnit->setAcceleration(sf::Vector2f(0, currentUnit->getAcceleration().y));
								currentUnit->setPosition(sf::Vector2f(tileBounds.left - originalBounds.width, currentUnit->getPosition().y));
								std::cout << "left" << std::endl;
							}

							//unit höger om tile
							else if (unitCenter.x > tileCenter.x)
							{
								currentUnit->setSpeed(sf::Vector2f(0, currentUnit->getSpeed().y));
								currentUnit->setAcceleration(sf::Vector2f(0, currentUnit->getAcceleration().y));
								currentUnit->setPosition(sf::Vector2f(tileBounds.left + tileBounds.width, currentUnit->getPosition().y));
								std::cout << "hoger" << std::endl;
							}
						}


						/*
						if (std::abs(distanceFromBottomToTop) < Constants::TileSize / 4 && std::abs(distanceFromBottomToTop) > 0)
						{
						currentUnit->setSpeed(sf::Vector2f(currentUnit->getSpeed().x, 0));
						currentUnit->setAcceleration(sf::Vector2f(currentUnit->getAcceleration().x, 0));
						currentUnit->setPosition(sf::Vector2f(currentUnit->getPosition().x, tileBounds.top - unitBounds.height));
						//std::cout << "distanceFromBottomToTop: " << distanceFromBottomToTop << std::endl;
						//int d = 0;
						}
						else if (std::abs(distanceFromTopToBottom) < Constants::TileSize / 4)
						{
						currentUnit->setSpeed(sf::Vector2f(currentUnit->getSpeed().x, 0));
						currentUnit->setAcceleration(sf::Vector2f(currentUnit->getAcceleration().x, 0));
						currentUnit->setPosition(sf::Vector2f(currentUnit->getPosition().x, tileBounds.top + tileBounds.height));
						}

						else if (std::abs(distanceFromLeftToRight) < Constants::TileSize / 4)
						{
						currentUnit->setSpeed(sf::Vector2f(0, currentUnit->getSpeed().y));
						currentUnit->setAcceleration(sf::Vector2f(0, currentUnit->getAcceleration().y));
						currentUnit->setPosition(sf::Vector2f(tileBounds.left + tileBounds.width, currentUnit->getPosition().y));
						}
						else if (std::abs(distanceFromRightToLeft) < Constants::TileSize / 4)
						{
						currentUnit->setSpeed(sf::Vector2f(0, currentUnit->getSpeed().y));
						currentUnit->setAcceleration(sf::Vector2f(0, currentUnit->getAcceleration().y));
						currentUnit->setPosition(sf::Vector2f(tileBounds.left - unitBounds.width, currentUnit->getPosition().y));
						}*/
					}
				}

				unitBounds = originalBounds;

			}
		}

		//Checks if the unit is standing at an edge and pushes it off the edge if it does
		if (!onGround)
		{
			if (currentUnit->getSpeed().y == 0)
			{
				if (leftEdge && currentUnit->getSpeed().x < glideSpeed && currentUnit->getSpeed().x > -glideSpeed)
				{
					currentUnit->setSpeed(sf::Vector2f(currentUnit->getSpeed().x + glideSpeed, currentUnit->getSpeed().y));
					std::cout << "what";
				}
				else if (rightEdge && currentUnit->getSpeed().x < glideSpeed && currentUnit->getSpeed().x > -glideSpeed)
				{
					currentUnit->setSpeed(sf::Vector2f(currentUnit->getSpeed().x - glideSpeed, currentUnit->getSpeed().y));
					std::cout << "yupp";
				}
			}	
		}

		currentUnit->updateAnimation(deltaTime);
	}
	/*
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
	}*/
}