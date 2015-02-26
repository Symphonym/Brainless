#include "Level.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "SoundPlayer.h"
#include "Constants.h"
#include "Utility.h"
#include "MovingPlatformItem.h"
#include <iostream>

Level::Level()
:
m_enableDarkness(false)
{
	// Load game resources

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
void Level::setDarkness(bool enabled)
{
	m_enableDarkness = enabled;
}

bool Level::loadLevelResources(const std::string &fileName)
{
	ResourceLoader::instance().loadResourceFile(fileName);
	m_backgrounds.clear();
	// Load backgrounds
	std::ifstream reader(fileName);

	bool hasLoaded = false;
	if (reader.is_open())
	{
		std::string line;
		while (std::getline(reader, line))
		{
			// Skip empty lines
			if (line.empty())
				continue;
			else if (line[0] == '#')
				continue;

			std::vector<std::string> stringData = Utility::splitString(line, ',');
			std::string resourceType = stringData[0].substr(1, stringData[0].size());

			// Add a new resource
			if (stringData[0][0] == '+')
			{

				if (resourceType == "Texture")
				{
					m_backgrounds.push_back(sf::Sprite(ResourceLoader::instance().retrieveTexture(stringData[2])));
				}
				else if (resourceType == "Music")
				{
					SoundPlayer::instance().playMusic(stringData[2], true);
					//TODO keep track of loaded music so it can be stopped
				}
				else
					continue;
			}
		}
		hasLoaded = true;
	}
	reader.close();

	
	return hasLoaded;
	
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
void Level::removeAllUnits()
{
	m_units.clear();
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
	for (std::size_t i = 0; i < m_units.size(); i++)
	{
		// TODO update units

	}
	for (std::size_t i = 0; i < m_items.size(); i++)
		m_items[i]->update(deltaTime, game);

	updateUnitCollision(deltaTime, game);
}

void Level::draw(const sf::View &cameraView, bool editorMode)
{
	m_tileMap->draw(cameraView, editorMode);
	//Find the screens percentual position on the screen
	float screen_x = (cameraView.getCenter().x - (cameraView.getSize().x* 0.5)) / (3840 - cameraView.getSize().x),
		screen_y = (cameraView.getCenter().y - (cameraView.getSize().y* 0.5)) / (1440 - cameraView.getSize().y);
	for (std::size_t i = 0; i < m_backgrounds.size(); i++)
	{
		//Move backgrounds accordingly
		m_backgrounds[i].setPosition(sf::Vector2f((3840 - m_backgrounds[i].getTexture()->getSize().x)*screen_x, (1440 - m_backgrounds[i].getTexture()->getSize().y)*screen_y));
		Renderer::instance().drawBackground(m_backgrounds[i]);
	}
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

bool Level::isDark() const
{
	return m_enableDarkness;
}

void Level::updateUnitCollision(float deltaTime, Game &game)
{
	for (unsigned int i = 0; i < m_units.size(); i++)
	{
		Unit* currentUnit = m_units[i].get();

		currentUnit->updateTask(deltaTime);



		currentUnit->updateMovement(Constants::Gravity, deltaTime);

		sf::FloatRect unitBounds = currentUnit->getCollisionRect();

		if (currentUnit->isMovementEnabled())
		{
			sf::Vector2i startIndex = m_tileMap->positionToIndex(sf::Vector2f(currentUnit->getPosition().x, currentUnit->getPosition().y));
			startIndex -= sf::Vector2i(1, 1);

			sf::Vector2i endIndex = m_tileMap->positionToIndex(
				sf::Vector2f(currentUnit->getPosition().x + currentUnit->getSize().x, currentUnit->getPosition().y + currentUnit->getSize().y));
			endIndex += sf::Vector2i(1, 1);

			startIndex.x = Utility::clampValue(startIndex.x, 0, Constants::MapWidth);
			startIndex.y = Utility::clampValue(startIndex.y, 0, Constants::MapHeight);

			endIndex.x = Utility::clampValue(endIndex.x, 0, Constants::MapWidth);
			endIndex.y = Utility::clampValue(endIndex.y, 0, Constants::MapHeight);


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

			bool inAir = true;
			bool tiltWalking = false;
			bool wasTiltWalking = currentUnit->getInTilt();

			bool collision = false;


			float unitLedgeOffset = 20;

#pragma region tiltCollision

			for (int i = 0; i < tiltRects.size(); i++)
				//for (int i = tiltRects.size(); i-- > 0; )
			{
				sf::FloatRect originalBounds = unitBounds;

				sf::FloatRect tileBounds = tiltRects[i];
				tileBounds.top += 1; // funkar bra i backen, bli fail när man går åt vänster från solid till tilt
				//	tileBounds.height += 1; //varför funkar inte detta då bra?
				sf::Vector2f tileCenter = sf::Vector2f(tileBounds.left + tileBounds.width / 2, tileBounds.top + tileBounds.height / 2);
				sf::Vector2f unitCenter = sf::Vector2f(unitBounds.left + unitBounds.width / 2, unitBounds.top + unitBounds.height / 2);


				//test_
				sf::FloatRect unitBottom = sf::FloatRect(unitBounds.left + unitLedgeOffset, unitBounds.top + unitBounds.height, unitBounds.width - unitLedgeOffset * 2, 1);
				sf::FloatRect tileTopBounds = sf::FloatRect(tileBounds.left, tileBounds.top - 1 /*motverkar "tophoppet"*/, tileBounds.width, 1);

				//notinAir
				if (currentUnit->getSpeed().y >= 0)
				if (!currentUnit->getInAir() && tileTopBounds.intersects(unitBottom))
				{
					inAir = false;
					tiltWalking = true;
					currentUnit->setTilt(true);
				}
				sf::Vector2f newPos = sf::Vector2f(currentUnit->getPosition().x, tileBounds.top - originalBounds.height + tileBounds.width - (originalBounds.left + originalBounds.width - tileBounds.left));

				////inte högre än tiltTile
				//while (newPos.y + 1 < tileBounds.top - originalBounds.height)
				//{
				//	//std::cout << "inte högre än tilblock" << std::endl;
				//	newPos.y++;
				//}

				//make bottom square
				unitBounds.top = unitBounds.top + unitBounds.height - unitBounds.width;
				unitBounds.height = unitBounds.width;

				float deltaX = abs(newPos.x - currentUnit->getPosition().x);

				////inte lägre än tileBlock
				//if (unitBounds.left + unitBounds.width - unitLedgeOffset < tileBounds.left + tileBounds.width && unitBounds.left + unitBounds.width >= tileBounds.left + tileBounds.width)
				//{
				//	//std::cout << "inte lägre än tilblock" << std::endl;
				//	newPos.y = tileBounds.top - originalBounds.height + 1;
				//}

				if (currentUnit->getSpeed().y >= 0) //if not going upwards
				if (tileBounds.intersects(unitBounds))
				{
					if ((unitBounds.left + originalBounds.width + unitBounds.top + unitBounds.height > tileCenter.x + tileCenter.y - 10) && unitBounds.left + unitBounds.width - unitLedgeOffset < tileBounds.left + tileBounds.width)
					{
						currentUnit->setSpeed(sf::Vector2f(currentUnit->getSpeed().x, 0));
						currentUnit->setAcceleration(sf::Vector2f(currentUnit->getAcceleration().x, 0));

						tiltWalking = true;
						currentUnit->setTilt(true);
						inAir = false;

					}
				}

				if (inAir == false)
				{
					if (unitBounds.left + unitBounds.width < tileBounds.left + tileBounds.width)
					{
						currentUnit->setPosition(newPos);
						tiltWalking = true;
						currentUnit->setTilt(true);
					}
				}

				unitBounds = originalBounds;
			}

#pragma endregion tiltCollision
			if (!tiltWalking)
			{
#pragma region solidCollision
				//std::cout << "solid collision" << std::endl;
				for (int i = 0; i < solidRects.size(); i++)
				{
					sf::FloatRect tileBounds = solidRects[i];
					sf::FloatRect tileTopBounds = sf::FloatRect(tileBounds.left, tileBounds.top, tileBounds.width, 1);
					sf::Vector2f tileCenter = sf::Vector2f(tileBounds.left + tileBounds.width / 2, tileBounds.top + tileBounds.height / 2);
					sf::Vector2f unitCenter = sf::Vector2f(unitBounds.left + unitBounds.width / 2, unitBounds.top + unitBounds.height / 2);
					sf::FloatRect unitBottom = sf::FloatRect(unitBounds.left + unitLedgeOffset, unitBounds.top + unitBounds.height, unitBounds.width - unitLedgeOffset * 2, 1);

					//notinAir
					if (currentUnit->getSpeed().y >= 0)
					{
						if (tileTopBounds.intersects(unitBottom))
						{
							inAir = false;
							currentUnit->setTilt(false);
						}
					}

					bool hasCollided = false;
					sf::FloatRect originalBounds = unitBounds;

					/*

					*/
					for (int c = 0; c < 2; c++)
					{
						//bottom square, height = width
						/*
							00
							00
							00
							11
							11
							*/
						if (c == 0)
						{
							unitBounds.top = unitBounds.top + unitBounds.height - unitBounds.width;
							unitBounds.height = unitBounds.width;
						}
						//top square
						/*
							11
							11
							00
							00
							00
							*/
						else
						{
							unitBounds.top = originalBounds.top;
						}

						// If tile isn't empty and is colliding with the unit
						if (tileBounds.intersects(unitBounds) && !hasCollided/* && !collision*/)
						{

							//	collision = true;
							hasCollided = true;

							unitCenter = sf::Vector2f(unitBounds.left + unitBounds.width / 2, unitBounds.top + unitBounds.height / 2);

							//Kolla om kollisionen är vertikal
							if (abs(unitCenter.x - tileCenter.x) * tileBounds.width < abs(unitCenter.y - tileCenter.y) * tileBounds.height)
							{
								//Kolla om tile under unit
								if (unitCenter.y < tileCenter.y)
								{
									//	if (currentUnit->getSpeed().y >= 0)
									//	{
									if (/*abs(currentUnit->getSpeed().x) > 50 ||*/ unitBottom.intersects(tileBounds))
									{
										currentUnit->collisionDown(tileBounds.top - originalBounds.height, 0, 0);
									}
									//	}
								}
								//kolla om tile ovanför unit
								else if (unitCenter.y > tileCenter.y && currentUnit->getSpeed().y < 0)
								{
									currentUnit->collisionUp(tileBounds.top + tileBounds.height, 0, 0);
								}
							}
							if (abs(unitCenter.x - tileCenter.x) > abs(unitCenter.y - tileCenter.y))
							{
								//tile höger om unit
								if (unitCenter.x < tileCenter.x /*&& currentUnit->getSpeed().x > 0*/)
								{
									if (currentUnit->getSpeed().x > 0)
										currentUnit->collisionRight(tileBounds.left - originalBounds.width, 0, 0);
									else
										currentUnit->collisionRight(tileBounds.left - originalBounds.width,
										currentUnit->getSpeed().x, 0);
								}

								//tile vänster om unit
								else if (unitCenter.x > tileCenter.x /*&& currentUnit->getSpeed().x < 0*/)
								{
									if (currentUnit->getSpeed().x < 0)
										currentUnit->collisionLeft(tileBounds.left + tileBounds.width, 0, 0);
									else
										currentUnit->collisionLeft(tileBounds.left + tileBounds.width,
										currentUnit->getSpeed().x, 0);
								}
							}
						}

					}
					unitBounds = originalBounds;
				}
				//meh funka plox
				if (currentUnit->updateCollision() && !currentUnit->getInAir()) inAir = false;
#pragma endregion solidCollision
			}

#pragma region platformCollision
			for (int i = 0; i < platformRects.size(); i++)
			{
				float platformCollisionHeight = 20;

				sf::FloatRect originalBounds = unitBounds;

				unitBounds.top = unitBounds.top + unitBounds.height - unitBounds.width;
				unitBounds.height = unitBounds.width;
				unitBounds.width = unitBounds.width - unitLedgeOffset * 2;
				unitBounds.left += unitLedgeOffset;

				sf::Vector2f tilePos = sf::Vector2f(platformRects[i].left, platformRects[i].top);
				sf::FloatRect tileBounds = sf::FloatRect(platformRects[i].left, platformRects[i].top - platformCollisionHeight, platformRects[i].width, platformCollisionHeight);
				sf::FloatRect unitBoundsBot = sf::FloatRect(unitBounds.left, unitBounds.top + unitBounds.height, unitBounds.width, -platformCollisionHeight);

				if (currentUnit->getSpeed().y >= 0)
				if (tileBounds.intersects(unitBoundsBot))
				{
					inAir = false;
					currentUnit->setPosition(sf::Vector2f(currentUnit->getPosition().x, tilePos.y - originalBounds.height));
				}

				unitBounds = originalBounds;
			}
#pragma endregion platformCollision

#pragma region itemCollision

			for (int i = 0; i < m_items.size(); i++)
			{

				if (m_items[i]->isCollidable())
				{
					sf::FloatRect tileBounds = m_items[i]->getCollisionBounds();
					sf::FloatRect tileTopBounds = sf::FloatRect(tileBounds.left, tileBounds.top, tileBounds.width, 1);
					sf::Vector2f tileCenter = sf::Vector2f(tileBounds.left + tileBounds.width / 2, tileBounds.top + tileBounds.height / 2);
					sf::Vector2f unitCenter = sf::Vector2f(unitBounds.left + unitBounds.width / 2, unitBounds.top + unitBounds.height / 2);
					sf::FloatRect unitBottom = sf::FloatRect(unitBounds.left + unitLedgeOffset, unitBounds.top + unitBounds.height - 5, unitBounds.width - unitLedgeOffset * 2, 1 + 5 + abs(m_items[i]->getSpeed().y * deltaTime));




					if (currentUnit->getSpeed().y >= 0)
					if (tileTopBounds.intersects(unitBottom))
					{
						inAir = false;
						currentUnit->setPosition(currentUnit->getPosition() + m_items[i]->getSpeed() * deltaTime);
						//		std::cout << "bottom intersect" << std::endl;
						//	std::cout << m_items[i]->getName() << std::endl;
					}

					bool hasCollided = false;
					sf::FloatRect originalBounds = unitBounds;
					if (m_items[i]->isSolid()) //notes Door är solid, platform är inte solid
						unitCenter = sf::Vector2f(unitBounds.left + unitBounds.width / 2, unitBounds.top + unitBounds.height / 2);
					else
						unitCenter = sf::Vector2f(unitBounds.left + unitBounds.width / 2, unitBounds.top + unitBounds.height);

					for (int c = 0; c < 2; c++)
					{
						if (c == 0)
						{
							unitBounds.top = unitBounds.top + unitBounds.height - unitBounds.width;
							unitBounds.height = unitBounds.width;

						}
						else
						{
							unitBounds.top = originalBounds.top;
						}

						// If tile isn't empty and is colliding with the unit
						if (tileBounds.intersects(unitBounds) && !hasCollided /*&& !collision*/)
						{

							//collision = true;
							hasCollided = true;

							//Kolla om kollisionen är vertikal
							if (abs(unitCenter.x - tileCenter.x) + (tileBounds.height / 2) < abs(unitCenter.y - tileCenter.y) + (tileBounds.width / 2))
							{
								//Kolla om unit är ovanför tile
								if (unitCenter.y  < tileCenter.y)
								{
									if (currentUnit->getSpeed().y >= 0)
									{
										if (abs(currentUnit->getSpeed().x) > 50 || unitBottom.intersects(tileBounds))
										{
											currentUnit->setSpeed(sf::Vector2f(currentUnit->getSpeed().x, 0));
											currentUnit->setAcceleration(sf::Vector2f(currentUnit->getAcceleration().x, 0));
											currentUnit->setPosition(sf::Vector2f(currentUnit->getPosition().x, tileBounds.top - originalBounds.height));


											//			std::cout << "above" << std::endl;
											//			std::cout << m_items[i]->getName() << std::endl;
										}
									}
								}
								else if (unitCenter.y > tileCenter.y && m_items[i]->isSolid())
								{
									currentUnit->setSpeed(sf::Vector2f(currentUnit->getSpeed().x, 0));
									currentUnit->setAcceleration(sf::Vector2f(currentUnit->getAcceleration().x, 0));
									currentUnit->setPosition(sf::Vector2f(currentUnit->getPosition().x, tileBounds.top + tileBounds.height + abs(m_items[i]->getSpeed().y * 100)));
									//		std::cout << "under" << std::endl;
									//		std::cout << m_items[i]->getName() << std::endl;
								}
							}
							else
							{
								//unit vänster om tile
								if (unitCenter.x < tileCenter.x&& m_items[i]->isSolid())
								{
									currentUnit->setSpeed(sf::Vector2f(0, currentUnit->getSpeed().y));
									currentUnit->setAcceleration(sf::Vector2f(0, currentUnit->getAcceleration().y));
									currentUnit->setPosition(sf::Vector2f(tileBounds.left - originalBounds.width, currentUnit->getPosition().y));
									//		std::cout << "left" << std::endl;
									//		std::cout << m_items[i]->getName() << std::endl;
								}

								//unit höger om tile
								else if (unitCenter.x > tileCenter.x&& m_items[i]->isSolid())
								{
									currentUnit->setSpeed(sf::Vector2f(0, currentUnit->getSpeed().y));
									currentUnit->setAcceleration(sf::Vector2f(0, currentUnit->getAcceleration().y));
									currentUnit->setPosition(sf::Vector2f(tileBounds.left + tileBounds.width, currentUnit->getPosition().y));
									//		std::cout << "hoger" << std::endl;aaaaa
									//		std::cout << m_items[i]->getName() << std::endl;
								}
							}
						}

					}
					unitBounds = originalBounds;
				}
			}

			m_items.erase(std::remove_if(m_items.begin(), m_items.end(), [&](const ItemPtr &item) -> bool
			{
				// Trigger collision events for items
				if (item->getCollisionBounds().intersects(currentUnit->getCollisionRect()))
				{
					return item->onCollisionWithUnit(*currentUnit, game);
				}
				else
					return false;
			}), m_items.end());

#pragma endregion itemCollision

			currentUnit->setInAir(inAir);
		}

		currentUnit->updateAnimation(deltaTime);
	}
}