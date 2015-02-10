#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <math.h>
//
#include "ResourceLoader.h"
#include "Constants.h"
#include "FileSave.h"

#include "Level.h"
#include "LevelSprite.h"
#include "TileMap.h"
#include "Tile.h"
#include "Item.h"
#include "ItemDatabse.h"
#include "Inventory.h"
#include "Unit.h"
#include "Zombie.h"
#include "ChasingZombie.h"
#include "WalkingZombie.h"
#include "IdleZombie.h"

#include "Utility.h"

void FileSave::saveMapText(Level &level, int levelNumber)
{
	std::ofstream writer("level" + std::to_string(levelNumber) + ".txt");

	// First write the player spawn point
	writer << level.getSpawnPos().x << std::endl;
	writer << level.getSpawnPos().y << std::endl;

	// Second write the number of rows the map has
	// This is how many lines the loader should read before moving onto items
	writer << Constants::MapHeight << std::endl;

	for (int y = 0; y < Constants::MapHeight; y++)
	{
		for (int x = 0; x < Constants::MapWidth; x++)
		{
			int tileType = static_cast<int>(level.getTileMap().getTile(x, y).getType());
			writer << tileType;

			if (x != Constants::MapWidth - 1)
				writer << ",";

		}
		writer << std::endl;
	}

	// Write the number of items to read
	writer << level.getItems().size() << std::endl;

	for (std::size_t i = 0; i < level.getItems().size(); i++)
	{
		Item& curItem = *level.getItems()[i].get();
		writer << curItem.getID() << "," << curItem.getSyncID() << "," << curItem.getPosition().x << "," << curItem.getPosition().y << std::endl;
	}

	// Write the number of sprites to read
	writer << level.getDecorations().size() << std::endl;

	for (std::size_t i = 0; i < level.getDecorations().size(); i++)
	{
		const LevelSprite& curSprite = level.getDecorations()[i];
		writer << curSprite.drawToForeground << "," << curSprite.textureName << "," << curSprite.sprite.getPosition().x << "," << curSprite.sprite.getPosition().y << std::endl;
	}

	// Write numbers of units
	writer << level.getUnits().size() << std::endl;

	for (std::size_t i = 0; i < level.getUnits().size(); i++)
	{
		Unit& curUnit = *level.getUnits()[i].get();
		writer << (int)curUnit.getUnitType() << ",";
		switch (curUnit.getUnitType())
		{
		case Unit::ID_IdleZombie:
			writer << curUnit.getPosition().x << "," << curUnit.getPosition().y << "," << (int)curUnit.getDirection() << std::endl;
			break;
		case Unit::ID_WalkingZombie:
			writer << curUnit.getPosition().x << "," << curUnit.getPosition().y << "," << (((WalkingZombie&) curUnit)).getWalkLenght() << std::endl;
			break;
		case Unit::ID_ChasingZombie:
			writer << curUnit.getPosition().x << "," << curUnit.getPosition().y << "," << (((ChasingZombie&)curUnit)).getWalkLenght() << std::endl;
			break;
		}
	}
}

bool FileSave::loadMapText(Level &level, int levelNumber)
{
	std::ifstream reader("level" + std::to_string(levelNumber) + ".txt");

	if (reader.is_open())
	{
		// Read spawn pos
		float spawnX = 0, spawnY = 0;
		reader >> spawnX >> spawnY;
		level.setSpawnPosition(sf::Vector2f(spawnX, spawnY));

		// Read row count
		int rowCount = 0;
		reader >> rowCount;

		// Load tile types
		for (int y = 0; y < rowCount; y++)
		{
			// Read a row
			std::string line;
			reader >> line;

			// Loop through the row
			std::vector<std::string> columns = Utility::splitString(line, ',');
			for (int x = 0; x < columns.size(); x++)
			{
				int tileType = Utility::stringToNumber<int>(columns[x]);
				level.getTileMap().getTile(x, y).setType(static_cast<Tile::TileTypes>(tileType));
			}
		}

		// Read item count
		int itemCount = 0;
		reader >> itemCount;

		for (std::size_t i = 0; i < itemCount; i++)
		{
			std::string line;
			reader >> line;

			std::vector<std::string> itemData = Utility::splitString(line, ',');

			// Read item data
			int itemID = Utility::stringToNumber<int>(itemData[0]);
			int syncID = Utility::stringToNumber<int>(itemData[1]);
			float posX = Utility::stringToNumber<float>(itemData[2]);
			float posY = Utility::stringToNumber<float>(itemData[3]);

			// Create item from data
			ItemDatabase::ItemPtr item = std::move(ItemDatabase::instance().extractItem(itemID));
			item->setSyncID(syncID);
			item->setPosition(sf::Vector2f(posX, posY));

			level.addItem(std::move(item));
		}

		// Read sprite count
		int spriteCount = 0;
		reader >> spriteCount;

		for (std::size_t i = 0; i < spriteCount; i++)
		{
			std::string line;
			reader >> line;

			std::vector<std::string> spriteData = Utility::splitString(line, ',');

			// Read sprite data
			bool drawToForeground = Utility::stringToNumber<bool>(spriteData[0]);
			std::string textureName = spriteData[1];
			float posX = Utility::stringToNumber<float>(spriteData[2]);
			float posY = Utility::stringToNumber<float>(spriteData[3]);

			// Create sprite from data
			LevelSprite levelSprite;
			levelSprite.drawToForeground = drawToForeground;
			levelSprite.textureName = textureName;
			levelSprite.sprite.setPosition(posX, posY);
			levelSprite.sprite.setTexture(ResourceLoader::instance().retrieveTexture(textureName));

			level.addDecoration(levelSprite);
		}

		// Read Unit count
		int unitCount = 0;
		reader >> unitCount;

		for (std::size_t i = 0; i < unitCount; i++)
		{
			std::string line;
			reader >> line;

			std::vector<std::string> unitData = Utility::splitString(line, ',');

			// Read unit data
			Unit::UnitType unitID = (Unit::UnitType)(Utility::stringToNumber<int>(unitData[0]));
			float posX = Utility::stringToNumber<float>(unitData[1]);
			float posY = Utility::stringToNumber<float>(unitData[2]);

			// Create unit & assign values, then add the unit to level
			Unit* temp;
			switch (unitID)
			{
			case Unit::ID_IdleZombie:
				temp = new IdleZombie(sf::Vector2f(posX, posY),(Unit::Direction)Utility::stringToNumber<int>(unitData[3]));
				temp->addTexture(ResourceLoader::instance().retrieveTexture("Zombie"));
				temp->updateAnimation(0);
				level.addUnit(std::move(Level::UnitPtr(temp)));
				break;
			case Unit::ID_WalkingZombie:
				temp = new WalkingZombie(sf::Vector2f(posX, posY), Utility::stringToNumber<int>(unitData[3]));
				temp->addTexture(ResourceLoader::instance().retrieveTexture("Zombie"));
				temp->updateAnimation(0);
				level.addUnit(std::move(Level::UnitPtr(temp)));
				break;
			case Unit::ID_ChasingZombie:
				temp = new ChasingZombie(sf::Vector2f(posX, posY), Utility::stringToNumber<int>(unitData[3]));
				temp->addTexture(ResourceLoader::instance().retrieveTexture("Zombie"));
				temp->updateAnimation(0);
				level.addUnit(std::move(Level::UnitPtr(temp)));
				break;
			}

		}

		return true;
	}

	return false;
}

void FileSave::saveInventory(Inventory &inventory)
{
	std::ofstream writer("inventory.txt");

	std::vector<const Item*> items = inventory.getInventoryItems();
	for (std::size_t i = 0; i < items.size(); i++)
	{
		items[i]->serialize(writer);
		//writer >> items[i]->getID() >>
	}
	writer.close();
}
bool FileSave::loadInventory(Inventory &inventory)
{
	std::ifstream reader("inventory.txt");

	bool opened = false;
	if (reader.is_open())
	{
		opened = true;

		std::string line;
		while (std::getline(reader, line))
		{
			// Skip empty lines
			if (line.empty())
				continue;

			int itemID = Utility::stringToNumber<int>(line);
			ItemDatabase::ItemPtr item = std::move(ItemDatabase::instance().extractItem(itemID));

			item->deserialize(reader);
			inventory.addItem(std::move(item));
		}
	}
	reader.close();
	return opened;
}

void FileSave::saveLevelProgress(Level &level, int levelNumber)
{
	std::ofstream writer("game_level" + std::to_string(levelNumber) + ".txt");
	for (std::size_t i = 0; i < level.getItems().size(); i++)
	{
		level.getItems()[i]->serialize(writer);
	}
	writer.close();

}
bool FileSave::loadLevelProgress(Level &level, int levelNumber)
{
	std::ifstream reader("game_level" + std::to_string(levelNumber) + ".txt");

	bool opened = false;
	if (reader.is_open())
	{
		level.removeAllItems();
		opened = true;

		std::string line;
		while (std::getline(reader, line))
		{
			// Skip empty lines
			if (line.empty())
				continue;

			int itemID = Utility::stringToNumber<int>(line);
			ItemDatabase::ItemPtr item = std::move(ItemDatabase::instance().extractItem(itemID));

			item->deserialize(reader);
			level.addItem(std::move(item));
		}
	}
	reader.close();
	return opened;
}

void FileSave::wipeProgress()
{
	// Rename the file first so that the game won't misstakenly interact with a non-existing file
	std::rename("inventory.txt", "inventory_old.txt");
	std::remove("inventory_old.txt");
	
	for (int i = 0; i < 10; i++)
	{

		std::string levelFile = "game_level" + std::to_string(i) + ".txt";
		std::string levelFileNew = "game_level" + std::to_string(i) + "_old.txt";
		std::rename(levelFile.c_str(), levelFileNew.c_str());
		std::remove(levelFileNew.c_str());
	}
}