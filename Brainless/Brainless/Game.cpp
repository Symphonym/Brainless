#include "FileSave.h"

#include "Game.h"
#include "Constants.h"
#include "Unit.h"
#include "Level.h"
#include "TileMap.h"
#include "Tile.h"



Game::Game()
{
}

void Game::loadFile()
{
	FileSave::loadMap(&m_level, 0);
}
void Game::saveFile()
{
	FileSave::saveMap(&m_level, 0);
}

void Game::loop()
{
	for (unsigned int i = 0; i < units.size(); i++)
	{
		/*
		Find next position
		- if (collision is free move, update status)
		--- collision with tiles
		--- collision with obstacles
		- else (move to contact, update status)*/
		//Find colliding boxes 
		float box_start_x = floor(units[i]->getPositionX());
		float box_end_x = ceil((units[i]->getPositionX() + units[i]->getWidth()));
		float box_start_y = floor(units[i]->getPositionY());
		float box_end_y = ceil((units[i]->getPositionY() + units[i]->getHeight())) + 1;
		//std::vector<Tile&> tiles;
		for (float x = box_start_x; x < box_end_x; x++)
		{
			for (float y = box_start_y; y < box_end_y; y++)
			{
				//Check for collision
				//tiles.push_back(m_level.getTileMap().getTile(x, y).collidesWith(NULL));
			}
		}
		//Move to collision 
	}
}