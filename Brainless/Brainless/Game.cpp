#include "Game.h"
#include "Constants.h"
#include "Unit.h"
#include "Level.h"
#include "TileMap.h"
#include "Tile.h"


Game::Game()
{
}

void Game::loop()
{
	for (int i = 0; i < units.size(); i++)
	{
		/*
		Find next position
		- if (collision is free move, update status)
		--- collision with tiles
		--- collision with obstacles
		- else (move to contact, update status)
		*/
		/*int new_x = units[i]->getX() + units[i]->getHspeed();
		int new_y = units[i]->getY() + units[i]->getVspeed();*/
		//Find colliding boxes 
		int box_start_x = floor(units[i]->getPositionX() / Constants::TileSize);
		int box_end_x = ceil((units[i]->getPositionX() + units[i]->getWidth()) / Constants::TileSize);
		int box_start_y = floor(units[i]->getPositionY() / Constants::TileSize);
		int box_end_y = ceil((units[i]->getPositionY() + units[i]->getHeight()) / Constants::TileSize) + 1;
		std::vector<Tile&> tiles;
		for (int x = box_start_x; x < box_end_x; x++)
		{
			for (int y = box_start_y; y < box_end_y; y++)
			{
				//Check for collision
				tiles.push_back(m_level.getTileMap().getTile(x, y).collidesWith(units[i]->getSprite().getGlobalBounds()));
			}
		}
		//Move to collision 

	}
}