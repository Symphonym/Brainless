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

void Level::setSpawnPosition(const sf::Vector2f &spawnPos)
{
	m_spawnPos = spawnPos;
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
void Level::removeAllItems()
{
	m_items.clear();
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
void Level::removeUnit(std::size_t index)
{
	if (index < m_units.size())
		m_units.erase(m_units.begin() + index);
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

void Level::update(float deltaTime, Game &game)
{
	const int unitSpeed = 600;
	for (std::size_t i = 0; i < m_units.size(); i++)
	{
		// TODO update units

	}
	for (std::size_t i = 0; i < m_items.size(); i++)
		m_items[i]->update(deltaTime, game);

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

const sf::Vector2f& Level::getSpawnPos() const
{
	return m_spawnPos;
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
			sf::Vector2f(currentUnit->getPosition().x + currentUnit->getSize().x, currentUnit->getPosition().y + currentUnit->getSize().y));
		endIndex += sf::Vector2i(1, 1);

		startIndex.x = Utility::clampValue(startIndex.x, 0, Constants::MapWidth - 1);
		startIndex.y = Utility::clampValue(startIndex.y, 0, Constants::MapHeight - 1);

		endIndex.x = Utility::clampValue(endIndex.x, 0, Constants::MapWidth - 1);
		endIndex.y = Utility::clampValue(endIndex.y, 0, Constants::MapHeight - 1);

		//std::cout << "STARTINDEX X " << startIndex.x << " STARTINDEX Y " << startIndex.y << " " << currentUnit->getInAir() << std::endl;

		std::vector<sf::FloatRect> solidRects;
		std::vector<sf::FloatRect> tiltRects;
		std::vector<sf::FloatRect> platformRects;

		for (int x = startIndex.x; x < endIndex.x; x++)
		{
			for (int y = startIndex.y; y < endIndex.y; y++)
			{
				if (m_tileMap->getTile(x, y).getType() != Tile::Nothing && !m_tileMap->getTile(x, y).getTilt() && !m_tileMap->getTile(x, y).getPlatform())
					solidRects.push_back(m_tileMap->getTile(x, y).getBounds());

				if (m_tileMap->getTile(x, y).getType() != Tile::Nothing && m_tileMap->getTile(x, y).getTilt() && !m_tileMap->getTile(x, y).getPlatform())
					tiltRects.push_back(m_tileMap->getTile(x, y).getBounds());

				if (m_tileMap->getTile(x, y).getType() != Tile::Nothing && !m_tileMap->getTile(x, y).getTilt() && m_tileMap->getTile(x, y).getPlatform())
					platformRects.push_back(m_tileMap->getTile(x, y).getBounds());
			}
		}

		currentUnit->setStatus(true);

		bool collision = false;

		float unitLedgeOffset = 20;

#pragma region tiltCollision
		for (int i = 0; i < tiltRects.size(); i++)
		{
			sf::FloatRect originalBounds = unitBounds;

			sf::FloatRect tileBounds = tiltRects[i];
			sf::Vector2f tileCenter = sf::Vector2f(tileBounds.left + tileBounds.width / 2, tileBounds.top + tileBounds.height / 2);
			sf::Vector2f unitCenter = sf::Vector2f(unitBounds.left + unitBounds.width / 2, unitBounds.top + unitBounds.height / 2);

			sf::Vector2f newPos = sf::Vector2f(currentUnit->getPosition().x, tileBounds.top - originalBounds.height + tileBounds.width - (originalBounds.left + originalBounds.width - tileBounds.left));

			while (newPos.y + 1 < tileBounds.top - originalBounds.height)
			{
				newPos.y++;
			}

			unitBounds.top = unitBounds.top + unitBounds.height - unitBounds.width;
			unitBounds.height = unitBounds.width;

			float deltaX = abs(newPos.x - currentUnit->getPosition().x);

			if (unitBounds.left + unitBounds.width - unitLedgeOffset < tileBounds.left + tileBounds.width && unitBounds.left + unitBounds.width >= tileBounds.left + tileBounds.width)
				newPos.y = tileBounds.top - originalBounds.height;

			if (currentUnit->getSpeed().y >= 0)
			if (tileBounds.intersects(unitBounds))
			{
				if ((unitBounds.left + originalBounds.width + unitBounds.top + unitBounds.height > tileCenter.x + tileCenter.y - 6) && unitBounds.left + unitBounds.width - unitLedgeOffset < tileBounds.left + tileBounds.width)
				{
					collision = true;
					currentUnit->setStatus(false);
					currentUnit->setPosition(newPos);
				}
			}

			if (currentUnit->getInAir() == false)
			{
				if (unitBounds.left + unitBounds.width < tileBounds.left + tileBounds.width)
				{
					currentUnit->setPosition(newPos);
				}
			}




			/*if (currentUnit->getSpeed().y >= 0)
			if (unitBounds.intersects(unitBounds) && !collision)
			{
			collision = true;

			unitCenter = sf::Vector2f(unitBounds.left + unitBounds.width / 2, unitBounds.top + unitBounds.height / 2);


			if (currentUnit->getInAir() == false)
			{
			if (unitBounds.left + unitBounds.width < tileBounds.left + tileBounds.width)
			{
			currentUnit->setPosition(newPos);

			if (originalBounds.left + originalBounds.width >= tileBounds.left + tileBounds.width - 1 && currentUnit->getSpeed().x > 0)
			{
			currentUnit->setPosition(sf::Vector2f(currentUnit->getPosition().x + 2, currentUnit->getPosition().y - 2));
			if (!tileBounds.intersects(unitBounds) && collision)
			currentUnit->setPosition(sf::Vector2f(currentUnit->getPosition().x, currentUnit->getPosition().y + 1));
			}

			if (originalBounds.left + originalBounds.width <= tileBounds.left + 1 && currentUnit->getSpeed().x > 0)
			currentUnit->setPosition(sf::Vector2f(currentUnit->getPosition().x + 2, currentUnit->getPosition().y - 1));
			}
			}

			if (unitBounds.left + unitBounds.width + unitBounds.top + unitBounds.height - 2 > tileCenter.x + tileCenter.y && unitBounds.left + unitBounds.width < tileBounds.left + tileBounds.width)
			{
			currentUnit->setSpeed(sf::Vector2f(currentUnit->getSpeed().x, 0));
			currentUnit->setAcceleration(sf::Vector2f(currentUnit->getAcceleration().x, 0));
			currentUnit->setPosition(sf::Vector2f(newPos));
			}
			}*/
			unitBounds = originalBounds;
		}
#pragma endregion tiltCollision

#pragma region solidCollision
		for (int i = 0; i < solidRects.size(); i++)
		{
			sf::FloatRect tileBounds = solidRects[i];
			sf::FloatRect tileTopBounds = sf::FloatRect(tileBounds.left, tileBounds.top, tileBounds.width, 1);
			sf::Vector2f tileCenter = sf::Vector2f(tileBounds.left + tileBounds.width / 2, tileBounds.top + tileBounds.height / 2);
			sf::Vector2f unitCenter = sf::Vector2f(unitBounds.left + unitBounds.width / 2, unitBounds.top + unitBounds.height / 2);
			sf::FloatRect unitBottom = sf::FloatRect(unitBounds.left + 20, unitBounds.top + unitBounds.height, unitBounds.width - 40, 1);

			if (currentUnit->getSpeed().y >= 0)
			if (tileTopBounds.intersects(unitBottom))
			{
				currentUnit->setStatus(false);
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
				if (tileBounds.intersects(unitBounds) && !hasCollided && !collision)
				{
					collision = true;
					hasCollided = true;

					unitCenter = sf::Vector2f(unitBounds.left + unitBounds.width / 2, unitBounds.top + unitBounds.height / 2);

					//Kolla om kollisionen är vertikal
					if (abs(unitCenter.x - tileCenter.x) * tileBounds.width < abs(unitCenter.y - tileCenter.y) * tileBounds.height)
					{
						//Kolla om unit är ovanför tile
						if (unitCenter.y < tileCenter.y)
						{
							if (currentUnit->getSpeed().y >= 0)
							{
								if (abs(currentUnit->getSpeed().x) > 50 || unitBottom.intersects(tileBounds))
								{
									currentUnit->setSpeed(sf::Vector2f(currentUnit->getSpeed().x, 0));
									currentUnit->setAcceleration(sf::Vector2f(currentUnit->getAcceleration().x, 0));
									currentUnit->setPosition(sf::Vector2f(currentUnit->getPosition().x, tileBounds.top - originalBounds.height));
									std::cout << "above" << std::endl;
								}
							}
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
				}

			}
			unitBounds = originalBounds;
		}
#pragma endregion solidCollision

#pragma region platformCollision
		for (int i = 0; i < platformRects.size(); i++)
		{
			float platformCollisionHeight = 20;

			sf::FloatRect originalBounds = unitBounds;

			unitBounds.top = unitBounds.top + unitBounds.height - unitBounds.width;
			unitBounds.height = unitBounds.width;
			unitBounds.width = unitBounds.width - unitLedgeOffset*2;
			unitBounds.left += unitLedgeOffset;

			sf::Vector2f tilePos = sf::Vector2f(platformRects[i].left, platformRects[i].top);
			sf::FloatRect tileBounds = sf::FloatRect(platformRects[i].left, platformRects[i].top - platformCollisionHeight, platformRects[i].width, platformCollisionHeight);
			sf::FloatRect unitBoundsBot = sf::FloatRect(unitBounds.left, unitBounds.top + unitBounds.height, unitBounds.width, -platformCollisionHeight);

			if (currentUnit->getSpeed().y >= 0)
			if (tileBounds.intersects(unitBoundsBot))
			{
				currentUnit->setStatus(false);
				currentUnit->setPosition(sf::Vector2f(currentUnit->getPosition().x, tilePos.y - originalBounds.height));
			}

			unitBounds = originalBounds;
		}
#pragma endregion platformCollision

		currentUnit->updateAnimation(deltaTime);
	}
}