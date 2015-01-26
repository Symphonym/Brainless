#include <fstream>
#include <string>

//
#include "Constants.h"
#include "FileSave.h"
#include "TileMap.h"
#include "Tile.h"

void FileSave::saveMap(TileMap* map, int stage_number)
{
	//File variables
	std::ofstream file_write; file_write.open(std::to_string(stage_number) +".fmap");
	const int file_size = 2 + Constants::MapWidth*Constants::MapHeight;
	unsigned char * file_content = new unsigned char[file_size];
	int file_at = 0;

	// - Save all tiles
	file_content[0] = Constants::MapWidth;
	file_content[1] = Constants::MapHeight;
	file_at = 2;
	for (int x = 0; x < Constants::MapWidth; x++)
	{
		for (int y = 0; y < Constants::MapHeight; y++)
		{
			file_content[file_at] = static_cast<int>(map->getTile(x, y).getType());
			file_at++;
		}
	}
	//write to file
	file_write.write((const char*)file_content, file_at);
	// - Save all items
	// byte - x
	// byte - y
	// byte - sprite
	// byte - code
	// - Save
}

void FileSave::loadMap(TileMap* map, int stage_number)
{
	//File variables
	std::ifstream file_read; file_read.open(std::to_string(stage_number) + ".fmap");
	file_read.seekg(0, file_read.end); //Find size of the file then go back to start
	const int file_size = file_read.tellg();
	file_read.seekg(0, file_read.beg);
	unsigned char * file_content = new unsigned char[file_size];
	int file_at = 0;

	//Read from file
	file_read.read((char*)file_content, file_size);

	int map_width = file_content[0];
	int map_ehight = file_content[1];
	file_at = 2;
	for (int x = 0; x < Constants::MapWidth; x++)
	{
		for (int y = 0; y < Constants::MapHeight; y++)
		{
			map->getTile(x, y).setType(static_cast<Tile::TileTypes>(file_content[file_at]));
			file_at++;
		}
	}
}
