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

#include "Utility.h"


void FileSave::saveMap(Level* stage, int stage_number)
{
	//Obtaining level parts
	TileMap& map = stage->getTileMap();
	//File variables
	std::ofstream file_write; file_write.open(std::to_string(stage_number) + ".fmap");
	const int file_size = 1000 + (2 + Constants::MapWidth*Constants::MapHeight) + 1 + stage->getItems().size();
	unsigned char * file_content = new unsigned char[file_size];
	int file_at = 0;

	// - Save all tiles to array
	file_content[0] = Constants::MapWidth;
	file_content[1] = Constants::MapHeight;
	file_at = 2;
	for (int x = 0; x < Constants::MapWidth; x++)
	{
		for (int y = 0; y < Constants::MapHeight; y++)
		{
			file_content[file_at] = static_cast<int>(map.getTile(x, y).getType());
			file_at++;
		}
	}
	//Save all items to array
	file_content[file_at] = stage->getItems().size(); file_at++;
	for (int i = 0; i < stage->getItems().size(); i++)
	{
		if (stage->getItems()[i]->getPosition().x < 0)
			file_content[file_at + 0] = 0; else file_content[file_at + 0] = -1;
		file_content[file_at + 1] = floor(stage->getItems()[i]->getPosition().x / 256);
		file_content[file_at + 2] = (int)(stage->getItems()[i]->getPosition().x) % 256;
		if (stage->getItems()[i]->getPosition().y < 0)
			file_content[file_at + 3] = 0; else file_content[file_at + 3] = -1;
		file_content[file_at + 4] = floor(stage->getItems()[i]->getPosition().y / 256);
		file_content[file_at + 5] = (int)(stage->getItems()[i]->getPosition().y) % 256;
		file_content[file_at + 6] = stage->getItems()[i]->getID();
		file_content[file_at + 7] = stage->getItems()[i]->getSyncID();
		file_at += 8;
	}
	//Save all textures to array
	file_content[file_at] = stage->getDecorations().size(); file_at++;
	for (int i = 0; i < stage->getDecorations().size(); i++)
	{
		//3xByte - indicating X, 3xByte - indicating Y
		if (stage->getDecorations()[i].sprite.getPosition().x < 0)
			file_content[file_at + 0] = 0; else file_content[file_at + 0] = 1;
		file_content[file_at + 1] = floor(stage->getDecorations()[i].sprite.getPosition().x / 256);
		file_content[file_at + 2] = (int)(stage->getDecorations()[i].sprite.getPosition().x) % 256;
		if (stage->getDecorations()[i].sprite.getPosition().y < 0)
			file_content[file_at + 3] = 0; else file_content[file_at + 3] = 1;
		file_content[file_at + 4] = floor(stage->getDecorations()[i].sprite.getPosition().y / 256);
		file_content[file_at + 5] = (int)(stage->getDecorations()[i].sprite.getPosition().y) % 256;
		//Byte - indicating foreground or background
		file_content[file_at + 6] = stage->getDecorations()[i].drawToForeground;
		file_at += 7;
		//Byte - indicating string lenght, string - indicating sprite name
		int string_lenght = stage->getDecorations()[i].textureName.length();
		std::string string = stage->getDecorations()[i].textureName;
		file_content[file_at] = string_lenght; file_at++;
		for (int j = 0; j < string_lenght; j++)
		{
			file_content[file_at] = string[j];
			file_at++;
		}
	}
	//write array to file
	file_write.write((const char*)file_content, file_at);
}

void FileSave::loadMap(Level* stage, int stage_number)
{
	//Obtaining level parts
	TileMap& map = stage->getTileMap();

	//File variables
	std::ifstream file_read; file_read.open(std::to_string(stage_number) + ".fmap");
	if (file_read.is_open())
	{ 
		file_read.seekg(0, file_read.end); //Find size of the file then go back to start
		const int file_size = file_read.tellg();
		file_read.seekg(0, file_read.beg);
		unsigned char * file_content = new unsigned char[file_size];
		int file_at = 0;

		//Read from file
		file_read.read((char*)file_content, file_size);

		//Loading tiles
		int map_width = file_content[0];
		int map_ehight = file_content[1];
		file_at = 2;
		for (int x = 0; x < Constants::MapWidth; x++)
		{
			for (int y = 0; y < Constants::MapHeight; y++)
			{
				map.getTile(x, y).setType(static_cast<Tile::TileTypes>(file_content[file_at]));
				file_at++;
			}
		}
		//Loading items
		int item_count = file_content[file_at]; file_at++;
		for (int i = 0; i < item_count; i++)
		{
			ItemDatabase::ItemPtr item = std::move(ItemDatabase::instance().extractItem(file_content[file_at + 6]));
			int item_x = file_content[file_at + 1] * 256 + file_content[file_at + 2];
			if (file_content[file_at + 0] == 0) item_x = -item_x;
			int item_y = file_content[file_at + 4] * 256 + file_content[file_at + 5];
			if (file_content[file_at + 3] == 0) item_y = -item_y;
			item->setPosition(sf::Vector2f(item_x, item_y));
			item->setSyncID(file_content[file_at + 7]);
			stage->addItem(std::move(item));
			file_at += 8;
		}
		//Loading textures
		int texture_count = file_content[file_at]; file_at++;
		for (int i = 0; i < texture_count; i++)
		{
			LevelSprite* texture = new LevelSprite();
			int texture_x = file_content[file_at + 1] * 256 + file_content[file_at + 2];
			if (file_content[file_at + 0] == 0) texture_x = -texture_x;
			int texture_y = file_content[file_at + 4] * 256 + file_content[file_at + 5];
			if (file_content[file_at + 3] == 0) texture_y -= texture_y;
			texture->sprite.setPosition(sf::Vector2f(texture_x, texture_y));
			texture->drawToForeground = file_content[file_at + 6];
			file_at += 7;
			std::string texture_string;
			for (int j = 1; j < file_content[file_at]+1; j++)
			{
				texture_string += file_content[file_at + j];
			}
			file_at += file_content[file_at]+1;
			texture->textureName = texture_string;
			texture->sprite.setTexture(ResourceLoader::instance().retrieveTexture(texture_string));
			stage->addDecoration(*texture);
		}
	}
}


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