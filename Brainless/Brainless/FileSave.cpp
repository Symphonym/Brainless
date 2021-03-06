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
#include "GhostItem.h"
#include "LadderItem.h"
#include "ItemDatabase.h"
#include "Inventory.h"
#include "Unit.h"
#include "Zombie.h"
#include "ChasingZombie.h"
#include "WalkingZombie.h"
#include "IdleZombie.h"
#include "ScriptedZombie.h"
#include "Game.h"
#include "Utility.h"

void FileSave::saveMapText(Level &level, int levelNumber)
{
	std::ofstream writer("save/level" + std::to_string(levelNumber) + ".txt");

	// First write the player spawn point
	writer << level.getSpawnPos().x << std::endl;
	writer << level.getSpawnPos().y << std::endl;

	// Then write whether or not it's a dark map
	writer << level.isDark() << std::endl;

	// Write music used for the level
	writer << level.getLevelMusicName() << std::endl;

	// Write name of backgrounds used for the level
	std::vector<std::string> backgroundNames = level.getBackgroundNames();
	writer << backgroundNames.size() << std::endl;
	for (std::size_t i = 0; i < backgroundNames.size(); i++)
		writer << backgroundNames[i] << std::endl;

	// Write the number of rows the map has
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
		writer << curItem.getID() << "," << curItem.getSyncID() << "," << curItem.getPosition().x << "," << curItem.getPosition().y;
		if (curItem.getName() == "Ghost")
		{
			writer << "," << ((GhostItem&)curItem).getGhostID();
		}
		else if (curItem.getName() == "Ladder")
		{
			writer << "," << ((LadderItem&)curItem).getLadderLenght() << "," << ((LadderItem&)curItem).getLadderTextureString();
		}
		writer << std::endl;
	}

	// Write the number of sprites to read
	writer << level.getDecorations().size() << std::endl;

	for (std::size_t i = 0; i < level.getDecorations().size(); i++)
	{
		const LevelSprite& curSprite = level.getDecorations()[i];
		writer << static_cast<int>(curSprite.layer) << "," << curSprite.textureName << "," << curSprite.sprite.getPosition().x << "," << curSprite.sprite.getPosition().y << std::endl;
	}

	// Write numbers of units
	writer << level.getUnits().size() << std::endl;

	for (std::size_t i = 0; i < level.getUnits().size(); i++)
	{
		Unit& curUnit = *level.getUnits()[i].get();
		writer << (int)curUnit.getRealUnitType() << "," << curUnit.getPosition().x << "," << curUnit.getPosition().y << "," << curUnit.getTextureID() << ","; //obs real
		switch (curUnit.getUnitType()) //obs decorator
		{
		case Unit::ID_IdleZombie:
			writer << (int)curUnit.getDirection();
			break;
		case Unit::ID_WalkingZombie:
			writer << curUnit.getWalkLength();
			break;
		case Unit::ID_ChasingZombie:
			writer << curUnit.getWalkLength();
			break;
		}
		writer << ',' << curUnit.getSyncID();
		if (curUnit.getRealUnitType() == Unit::ID_ScriptZombie)
		{
			writer << "," << /*6*/(((ScriptedZombie&)curUnit)).getScriptID() << "," <</*7*/ (int)(((ScriptedZombie&)curUnit)).getUnitType();
		}
		writer << std::endl;
	}

	writer.close();
}

bool FileSave::loadMapText(Level &level, int levelNumber)
{
	std::ifstream reader("save/level" + std::to_string(levelNumber) + ".txt");

	if (reader.is_open())
	{
		// Read spawn pos
		float spawnX = 0, spawnY = 0;
		reader >> spawnX >> spawnY;
		level.setSpawnPosition(sf::Vector2f(spawnX, spawnY));

		// Read darkness value
		bool darkness = false;
		reader >> darkness;
		level.setDarkness(darkness);

		// Reader level music name
		std::string musicName = "";
		reader >> musicName;
		level.setLevelMusic(musicName);

		// Read background names
		int backgroundCount = 0;
		reader >> backgroundCount;
		for (int i = 0; i < backgroundCount; i++)
		{
			std::string backgroundName = "";
			reader >> backgroundName;
			level.addBackground(backgroundName);
		}

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
			for (std::size_t x = 0; x < columns.size(); x++)
			{
				int tileType = Utility::stringToNumber<int>(columns[x]);
				level.getTileMap().getTile(x, y).setType(static_cast<Tile::TileTypes>(tileType));
			}
		}

		// Read item count
		int itemCount = 0;
		reader >> itemCount;

		for (int i = 0; i < itemCount; i++)
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

			// Add speciall variables
			if (item->getName() == "Ghost")
			{
				(dynamic_cast<GhostItem*>(item.get()))->setGhostID(Utility::stringToNumber<float>(itemData[4]));
			}
			else if (item->getName() == "Ladder")
			{
				if (Utility::stringToNumber<float>(itemData[4])>0)
					(dynamic_cast<LadderItem*>(item.get()))->setLadderTexture(Utility::stringToNumber<float>(itemData[4]),itemData[5]);
			}
			level.addItem(std::move(item));
		}

		// Read sprite count
		std::size_t spriteCount = 0;
		reader >> spriteCount;

		for (std::size_t i = 0; i < spriteCount; i++)
		{
			std::string line;
			reader >> line;

			std::vector<std::string> spriteData = Utility::splitString(line, ',');

			// Read sprite data
			LevelSpriteLayers layer = static_cast<LevelSpriteLayers>(Utility::stringToNumber<int>(spriteData[0]));
			std::string textureName = spriteData[1];
			float posX = Utility::stringToNumber<float>(spriteData[2]);
			float posY = Utility::stringToNumber<float>(spriteData[3]);

			// Create sprite from data
			LevelSprite levelSprite;
			levelSprite.layer = layer;
			levelSprite.textureName = textureName;
			levelSprite.sprite.setPosition(posX, posY);
			levelSprite.sprite.setTexture(ResourceLoader::instance().retrieveTexture(textureName));

			level.addDecoration(levelSprite);
		}

		// Read Unit count
		std::size_t unitCount = 0;
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
				//unitData[4] "special zombie data" unitData[3] " texture"
			case Unit::ID_IdleZombie:
				temp = new IdleZombie(sf::Vector2f(posX, posY), (Unit::Direction)Utility::stringToNumber<int>(unitData[4]), Utility::stringToNumber<int>(unitData[3]), Utility::stringToNumber<int>(unitData[5]));
				temp->updateAnimation(0);
				level.addUnit(std::move(Level::UnitPtr(temp)));
				break;
			case Unit::ID_WalkingZombie:
				temp = new WalkingZombie(sf::Vector2f(posX, posY), Utility::stringToNumber<int>(unitData[4]), Utility::stringToNumber<int>(unitData[3]), Utility::stringToNumber<int>(unitData[5]));
				temp->updateAnimation(0);
				level.addUnit(std::move(Level::UnitPtr(temp)));
				break;
			case Unit::ID_ChasingZombie:
				temp = new ChasingZombie(sf::Vector2f(posX, posY), Utility::stringToNumber<int>(unitData[4]), Utility::stringToNumber<int>(unitData[3]), Utility::stringToNumber<int>(unitData[5]));
				temp->updateAnimation(0);
				level.addUnit(std::move(Level::UnitPtr(temp)));
				break;
			case Unit::ID_ScriptZombie:
				Zombie* scriptedZombie;
				switch (Utility::stringToNumber<int>(unitData[7]))
				{
				case Unit::ID_IdleZombie:

					scriptedZombie = new IdleZombie(sf::Vector2f(posX, posY), (Unit::Direction)Utility::stringToNumber<int>(unitData[4]), Utility::stringToNumber<int>(unitData[3]), Utility::stringToNumber<int>(unitData[5]));
					scriptedZombie->updateAnimation(0);
					break;
				case Unit::ID_WalkingZombie:
					scriptedZombie = new WalkingZombie(sf::Vector2f(posX, posY), Utility::stringToNumber<int>(unitData[4]), Utility::stringToNumber<int>(unitData[3]), Utility::stringToNumber<int>(unitData[5]));
					scriptedZombie->updateAnimation(0);
					break;
				case Unit::ID_ChasingZombie:
					scriptedZombie = new ChasingZombie(sf::Vector2f(posX, posY), Utility::stringToNumber<int>(unitData[4]), Utility::stringToNumber<int>(unitData[3]), Utility::stringToNumber<int>(unitData[5]));
					scriptedZombie->updateAnimation(0);
					break;
				default:
					scriptedZombie = nullptr; //do krasch, make compiler happy
					break;
				}
				temp = new ScriptedZombie(scriptedZombie, Utility::stringToNumber<int>(unitData[6]), &level);
				level.addUnit(std::move(Level::UnitPtr(temp)));
				break;
			default:
				break;
			}
		}

	

		reader.close();
		return true;
	}

	reader.close();
	return false;
}

void FileSave::saveInventory(Inventory &inventory)
{
	std::ofstream writer("save/inventory.txt");

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
	std::ifstream reader("save/inventory.txt");

	bool opened = false;
	if (reader.is_open())
	{
		opened = true;
		inventory.emptyInventory();

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
	std::ofstream writer("save/game_level" + std::to_string(levelNumber) + ".txt");

	// Write amount of items and then serialize all items
	writer << level.getItems().size() << std::endl;
	for (std::size_t i = 0; i < level.getItems().size(); i++)
	{
		level.getItems()[i]->serialize(writer);
	}

	// Write amount of units and then serialize all units
	writer << level.getUnits().size() << std::endl;
	for (std::size_t i = 0; i < level.getUnits().size(); i++)
	{
		// Do not serialize player
		if (level.getUnits()[i]->getUnitType() != Unit::ID_Player)
			level.getUnits()[i]->serialize(writer);
	}

	writer.close();

}
bool FileSave::loadLevelProgress(Level &level, int levelNumber)
{
	std::ifstream reader("save/game_level" + std::to_string(levelNumber) + ".txt");

	bool opened = false;
	if (reader.is_open())
	{
		level.removeAllItems();
		level.removeAllUnits();
		opened = true;

		// Load items
		int itemCount = 0;
		reader >> itemCount;

		for (int i = 0; i < itemCount; i++)
		{
			int itemID = 0;
			reader >> itemID;
			ItemDatabase::ItemPtr item = std::move(ItemDatabase::instance().extractItem(itemID));

			item->deserialize(reader);
			level.addItem(std::move(item));
		}

		// Load units
		int unitCount = 0;
		reader >> unitCount;

		for (int i = 0; i < unitCount; i++)
		{
			int unitID = 0;
			reader >> unitID;

			Unit::UnitType unitType = static_cast<Unit::UnitType>(unitID);
			Level::UnitPtr unit;

			switch (unitType)
			{
			case Unit::ID_IdleZombie:
				unit = Level::UnitPtr(new IdleZombie(sf::Vector2f(0, 0), Unit::dir_left, 0));
				unit->updateAnimation(0);
				break;
			case Unit::ID_WalkingZombie:
				unit = Level::UnitPtr(new WalkingZombie(sf::Vector2f(0, 0), 0, 0));
				unit->updateAnimation(0);
				break;
			case Unit::ID_ChasingZombie:
				unit = Level::UnitPtr(new ChasingZombie(sf::Vector2f(0, 0), 0, 0));
				unit->updateAnimation(0);
				break;
			case Unit::ID_ScriptZombie:
				unit = Level::UnitPtr(new ScriptedZombie(nullptr, 0, &level));
				break;
			}
			if (unit) //not player I guess?
			{
				unit->deserialize(reader);
				level.addUnit(std::move(unit));
			}
		}
	}
	reader.close();
	return opened;
}


void FileSave::saveGameData(Game &game)
{
	std::ofstream writer("save/game.txt");

	writer << game.getLevelIndex() << std::endl;
	writer << game.getSavedZombieCount() << std::endl;
	writer << game.getSpiritBar().getValue() << std::endl;
	writer << game.getSpiritBar().getMaxValue() << std::endl;
	writer << game.getPlayer().getHealth() << std::endl;

	writer.close();
}
void FileSave::loadGameData(Game &game)
{
	//OBS denna �r nu h�rdkodat s� att game.changeLevel() har laddat f�rsta raden redan.

	std::ifstream reader("save/game.txt");

	if (reader.is_open())
	{
		//read current level
		int a;
		reader >> a;

		game.clearSavedZombies();

		int savedZombieCount = 0;
		reader >> savedZombieCount;
		game.addSavedZombie(savedZombieCount);

		int spiritValue = 0, spiritMax = 0;
		reader >> spiritValue >> spiritMax;
		game.getSpiritBar().setMaxValue(spiritMax);
		game.getSpiritBar().setValue(spiritValue);

		int playerHealth = 0;
		reader >> playerHealth;
		game.getPlayer().setHealth(playerHealth);
	}


	reader.close();
}

int FileSave::findHighestSavedLevelIndex()
{
	int highestIndex = 0;
	for (int i = 0; i < 10; i++)
	{
		std::ifstream reader("save/game_level" + std::to_string(i) + ".txt");

		if (reader.is_open())
			highestIndex = i;
		else
			break;

		reader.close();
	}

	return highestIndex;
}


void FileSave::wipeProgress()
{
	// Rename the file first so that the game won't misstakenly interact with a non-existing file
	std::rename("save/inventory.txt", "save/inventory_old.txt");
	std::remove("save/inventory_old.txt");

	std::rename("save/game.txt", "save/game_old.txt");
	std::remove("save/game_old.txt");

	for (int i = 0; i < 10; i++)
	{

		std::string levelFile = "save/game_level" + std::to_string(i) + ".txt";
		std::string levelFileNew = "save/game_level" + std::to_string(i) + "_old.txt";
		std::rename(levelFile.c_str(), levelFileNew.c_str());
		std::remove(levelFileNew.c_str());
	}
}