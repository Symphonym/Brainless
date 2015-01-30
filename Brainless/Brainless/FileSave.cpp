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
			stage->getItems().push_back(std::move(item));
			file_at += 8;
		}
		//Loading textures
		int texture_count = file_content[file_at]; file_at++;
		for (int i = 0; i < texture_count; i++)
		{
			LevelSprite* texture = new LevelSprite();
			stage->getDecorations().push_back(*texture);
			int texture_x = file_content[file_at + 1] * 256 + file_content[file_at + 2];
			if (file_content[file_at + 0] == 0) texture_x = -texture_x;
			int texture_y = file_content[file_at + 4] * 256 + file_content[file_at + 5];
			if (file_content[file_at + 3] == 0) texture_y -= texture_y;
			texture->sprite.setPosition(sf::Vector2f(texture_x, texture_y));
			texture->drawToForeground = file_content[file_at + 6];
			file_at += 7;
			std::string texture_string;
			for (int j = 0; j < file_content[file_at]; j++)
			{
				texture_string += file_content[file_at + j];
			}
			file_at += file_content[file_at];
			texture->textureName = texture_string;
			//texture->sprite.setTexture(ResourceLoader::instance().retrieveTexture(texture_string));
		}
	}
}
