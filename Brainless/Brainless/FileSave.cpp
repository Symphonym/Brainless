#include <ostream>
#include <fstream>
#include "FileSave.h"
#include "Constants.h"
#include "TileMap.h"
#include "Tile.h"

void FileSave::saveMap(TileMap& map)
{
	int map_width = Constants::MapWidth, map_height = Constants::MapHeight;
	//File variables
	std::ofstream file_write;
	file_write.open("Map1.fmap");
	const int file_size = 2 + map_width*map_height;
	unsigned char * file_content = new unsigned char[file_size];
	int file_at = 0;
	// - Save all tiles
	file_content[0] = map_width;
	file_content[1] = map_height;
	file_at = 2;
	for (int x = 0; x < map_width; x++)
	{
		for (int y = 0; y < map_height; y++)
		{
			file_content[file_at] = static_cast<int>(map.getTile(x, y).getType());
			file_at++;
		}
	}
	file_write.write((const char*)file_content, file_at);
	// - Save all items
	//2xbyte x
	//2xbyte y
	// - Save
}

void FileSave::loadMap()
{
	std::ifstream file_read("test.txt", std::ifstream::binary);
	file_read.open("Map1.fmap");

	//Find size then go back to start
	file_read.seekg(0, file_read.end);
	const int file_size = file_read.tellg();
	file_read.seekg(0, file_read.beg);

	unsigned char * file_content = new unsigned char[file_size];
	file_read.read((char*)file_content, file_size);

	int map_width = file_content[0];
	int map_ehight = file_content[1];

	for (int i = 0; i < file_size; i++)
	{
		//std::cout << i << ":" << (int)file_content[i] << std::endl;
	}
}