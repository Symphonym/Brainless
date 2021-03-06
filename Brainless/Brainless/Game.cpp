#include <iostream>
#include <algorithm>
#include <stdlib.h>

#include "Game.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Constants.h"
#include "Utility.h"
#include "Unit.h"
#include "Player.h"
#include "IdleZombie.h"
#include "WalkingZombie.h"
#include "Level.h"
#include "FileSave.h"
#include "TileMap.h"
#include "Tile.h"
#include "ConversationBox.h"
#include "SoundPlayer.h"
#include "Notification.h"
#include "Cursor.h"
#include "StateMachine.h"
#include "PauseMenu.h"
#include "Constants.h"
#include "ParticleSystem.h"
#include "Credits.h"
#include "Button.h"

Game::Game(StateMachine &machine)
:
State(machine),
m_savedZombies(0),
m_levelIndex(0),
m_player(nullptr)
{
	m_camera = m_window.getDefaultView();

	// Load game resources
	ResourceLoader::instance().loadResourceFile("loadfiles/ResourceLoad_Game.txt");

	// Set game shader
	Renderer::instance().plugShader(ResourceLoader::instance().retrieveShader("DarknessShader"));

	m_inventoryButton = std::unique_ptr<GUIElement>(new Button(
		ResourceLoader::instance().retrieveTexture("ButtonBag_Normal"),
		ResourceLoader::instance().retrieveTexture("ButtonBag_Pressed"),
		sf::Vector2f(0, m_window.getSize().y - 200.f)));


	m_levelTransition = std::unique_ptr<LevelTransition>(new LevelTransition(*this));
	m_inventory = new Inventory();
	m_popup = new PopUpMenu();
	m_popup->setItemCallback([&](Item* itm, PopUpMenu::InteractTypes type) -> void
	{
		// Can't do anything if you're talking to someone
		if (ConversationBox::instance().isShown())
			return;

		sf::Vector2f playerCenter = sf::Vector2f(m_player->getPosition().x + m_player->getSize().x / 2.f, m_player->getPosition().y + m_player->getSize().y / 2.f);
		sf::Vector2f itemCenter = sf::Vector2f(
			itm->getInteractBounds().left + itm->getInteractBounds().width / 2.f,
			itm->getInteractBounds().top + itm->getInteractBounds().height / 2.f);

		sf::Vector2f distVec = itemCenter - playerCenter;
		distVec.x = std::abs(distVec.x);
		distVec.y = std::abs(distVec.y);

		//std::cout << "DistX: " << distVec.x << std::endl;
		//std::cout << "DistY: " << distVec.y << std::endl;

		//std::cout << "InteractX: " << itm->getInteractDistance().x << std::endl;
		//std::cout << "InteractY: " << itm->getInteractDistance().y << std::endl;

		if (type == PopUpMenu::InteractTypes::Pickup)
		{
			// Item too far away
			if (distVec.x > itm->getInteractDistance().x || distVec.y > itm->getInteractDistance().y)
			{
				Notification::instance().write("It's too far away");
				return;
			}

			if (itm->isLootable())
			{
				itm->onPickUp();
				m_inventory->addItem(std::move(m_level.removeItem(itm)));
			}
			else
				Notification::instance().write(itm->getPickupString());

		}
		else if (type == PopUpMenu::InteractTypes::Use)
		{
			// Item too far away
			if (distVec.x > itm->getInteractDistance().x || distVec.y > itm->getInteractDistance().y)
			{
				Notification::instance().write("It's too far away");
				return;
			}

			if (itm->isUsable())
			{
				itm->onUse(*this);

				// Sync use functionality for sync
				for (std::size_t i = 0; i < m_level.getItems().size(); i++)
				{
					if (m_level.getItems()[i]->getSyncID() >= 0 && m_level.getItems()[i]->getSyncID() == itm->getSyncID())
						m_level.getItems()[i]->onSyncedWith(*itm);
				}
			}
			else
				Notification::instance().write(itm->getUseString());
		}
		else if (type == PopUpMenu::InteractTypes::Examine)
		{
			itm->onExamine();
			Notification::instance().write(itm->getExamineString());
		}
	});
	m_spiritBar = new SpiritBar();
	m_spiritBar->setMaxValue(156);
	m_spiritBar->setPosition(sf::Vector2f(
		5,
		m_window.getSize().y - m_spiritBar->getSize().y - 5.f));

	m_healthBar = new HealthBar();

	// Load a default map with nothing but ground tiles
	TileMap::TileMapLayout layout;
	for (int x = 0; x < Constants::MapWidth; x++)
	{
		layout.push_back(std::vector<Tile::TileTypes>());
		for (int y = 0; y < Constants::MapHeight; y++)
			layout[x].push_back(Tile::Ground);
	}

	// TEST CODE FOR LOADING DEFAULT LEVEL
	changeLevel(-1, false);
}
Game::~Game()
{
	delete m_inventory;
	delete m_popup;
	delete m_spiritBar;
	delete m_healthBar;
	//Clear units
}

void Game::onPlay()
{
	SoundPlayer::instance().playMusic(m_level.getLevelMusicName(), true, 20);
}
void Game::onStop()
{
	SoundPlayer::instance().stopMusic(m_level.getLevelMusicName());
}

void Game::addSavedZombie(int count)
{
	m_savedZombies += count;
	saveGame();
}
void Game::clearSavedZombies()
{
	m_savedZombies = 0;
}

void Game::lootItem(Inventory::ItemPtr item)
{
	m_inventory->addItem(std::move(item));
}

void Game::changeLevel(int levelIndex, bool swapPosition)
{
	if (levelIndex == -1)
	{
		std::ifstream reader("save/game.txt"); 
		if (reader.is_open())
		{
			reader >> levelIndex;
		}
		else
		{
			levelIndex = 0;
		}
		reader.close();
	}
		if (levelIndex < 0)
			return;

		// TODO REMOVE THIS PLACEHOLDER CODE THAT ENDS GAME WHEN LOADING LEVEL 10
		if (levelIndex >= 10)
		{
			m_machine.popState();
			m_machine.pushState<Credits>();
			return;
		}



		sf::Vector2f player_location(-60, -60);
		// Remeber old player location
		if (swapPosition && m_player != nullptr)
		{
			//Swap sides
			player_location = m_player->getPosition();
			if (player_location.x < (Constants::MapWidth*0.5)*Constants::TileSize)
				player_location.x = (Constants::MapWidth - 1)*Constants::TileSize;
			else
				player_location.x = (1)*Constants::TileSize;
			//player_location.x = Utility::clampValue<float>((Constants::MapWidth)*Constants::TileSize - (player_location.x), Constants::TileSize, (Constants::MapWidth - 1)*Constants::TileSize);
		}

		// Auto save level before changing level
		if (m_player != nullptr) // If this is a level change from a menu, the player won't already exist
			saveGame();

		// Stop previous level music if there is any
		SoundPlayer::instance().stopMusic(m_level.getLevelMusicName());

		// Reset level
		m_level.reset();
		m_levelIndex = levelIndex;

		//Load level resources 
		m_level.loadLevelResources("loadfiles/ResourceLoad_Level" + std::to_string(m_levelIndex) + ".txt");

		// Reload level
		FileSave::loadMapText(m_level, m_levelIndex); // All tiles are cleared in here
		FileSave::loadLevelProgress(m_level, m_levelIndex); // All units are cleared from level here
		FileSave::loadInventory(*m_inventory);

		if (m_level.getLevelMusicName() != Constants::NoMusicProvidedString)
			SoundPlayer::instance().playMusic(m_level.getLevelMusicName(), true, 20);


		//Set player to start position
		if (player_location == sf::Vector2f(-60, -60))
		{
			player_location = m_level.getSpawnPos();
		}
		else
		{
			int temp_y = Constants::MapHeight - 1;
			while (m_level.getTileMap().getTile(floor(player_location.x / Constants::TileSize), temp_y).getType() != Tile::Nothing)
			{
				temp_y--;
			}
			player_location.y = (temp_y - 1)*Constants::TileSize;
		}
		// Add player to level
		m_player = static_cast<Player*>(m_level.addUnit(Level::UnitPtr(new Player(player_location))));
		FileSave::loadGameData(*this); // Load core game data for player
		
		if (m_player->getHealth() == -99) //If the player died reset their hp
		{
			m_player->setHealth(m_player->getMaxHealth());
		}
		// Auto save when loading a new level
		saveGame();

		//temp, texture borde laddas in p� annat s�tt.
		m_player->addTexture(ResourceLoader::instance().retrieveTexture("PlayerSheet"));
		m_player->addTexture(ResourceLoader::instance().retrieveTexture("PlayerSheetJump"));
		m_player->addTexture(ResourceLoader::instance().retrieveTexture("PlayerSheetJumpRun"));
	
}
void Game::changeLevelTransition(int levelIndex, bool swapPosition)
{
	if (levelIndex < 0)
		return;

	m_levelTransition->startTransition(levelIndex, swapPosition, 2.f);
}

void Game::addCamera(const sf::View &camera)
{
	m_extraCameras.push_back(camera);
}

void Game::saveGame()
{
	FileSave::saveInventory(*m_inventory);
	FileSave::saveLevelProgress(m_level, m_levelIndex);
	FileSave::saveGameData(*this);
}

Player& Game::getPlayer()
{
	return *m_player;
}
Level& Game::getLevel()
{
	return m_level;
}
int Game::getLevelIndex()
{
	return m_levelIndex;
}
SpiritBar& Game::getSpiritBar()
{
	return *m_spiritBar;
}

int Game::getSavedZombieCount() const
{
	return m_savedZombies;
}

bool Game::inventoryContains(const std::string &itemName) const
{
	for (std::size_t i = 0; i < m_inventory->getInventoryItems().size(); i++)
	{
		const Item &item = *m_inventory->getInventoryItems()[i];

		if (item.getName() == itemName)
			return true;
	}

	return false;
}

bool Game::holdingItem() const
{
	return m_inventory->holdingItem();
}

void Game::events(const sf::Event &event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::N && false)
		{
			Notification::instance().write("Game saved successfully!");
			saveGame();
		}
		else if (event.key.code == sf::Keyboard::Escape)
		{
			State* pauseState = m_machine.pushState<PauseMenu>();
			if (PauseMenu *pauseMenu = dynamic_cast<PauseMenu*>(pauseState))
			{
				pauseMenu->giveGame(*this);
			}
		}

	}

	// Pump events to everything that needs it
	// Disable game input when conversation is ongoing
	if (!ConversationBox::instance().isShown())
	{
		m_popup->events(event, *this);
		m_inventory->events(event, *this);
	}
	else
		ConversationBox::instance().events(event, *this);
}
void Game::update(float deltaTime)
{
	sf::Shader *currentShader = Renderer::instance().getCurrentShader();
	if (currentShader)
	{
		currentShader->setParameter("intensityValue", Utility::clampValue<float>(1.f - (static_cast<float>(m_savedZombies) / static_cast<float>(Constants::TotalBrainCount))*0.9,0.f,0.9f));
		currentShader->setParameter("enableDarkness", static_cast<int>(m_level.isDark()));
		currentShader->setParameter("enableLightSource", 0); // False until something else sets it
	}

	if (m_inventoryButton->getReleased(m_window))
	{
		//SoundPlayer::instance().playSound("inventory_open", getWindow().getView().getCenter());
		SoundPlayer::instance().playSound("inventory_open", getPlayer().getPosition(), 50.0f);
		m_inventory->toggleVisible();
	}


	// Update game logic and input, if not paused
	// Disable game input when conversation is ongoing
	if (!ConversationBox::instance().isShown())
	{
		m_level.update(deltaTime, *this);
		m_inventory->update(deltaTime, *this);
		m_popup->update(*this,
			sf::Vector2f(m_player->getPosition().x + m_player->getSize().x / 2.f, m_player->getPosition().y + m_player->getSize().y / 2.f));
	}
	m_camera.setCenter(m_player->getCameraPosition().x, m_player->getCameraPosition().y);

	if (m_camera.getCenter().y > Constants().MapHeight * Constants().TileSize - (m_camera.getSize().y / 2))
		m_camera.setCenter(m_camera.getCenter().x, Constants().MapHeight * Constants().TileSize - (m_camera.getSize().y / 2));

	if (m_camera.getCenter().y < (m_camera.getSize().y / 2))
		m_camera.setCenter(m_camera.getCenter().x, (m_camera.getSize().y / 2));

	if (m_camera.getCenter().x > Constants().MapWidth * Constants().TileSize - (m_camera.getSize().x / 2))
		m_camera.setCenter(Constants().MapWidth * Constants().TileSize - (m_camera.getSize().x / 2), m_camera.getCenter().y);

	if (m_camera.getCenter().x < (m_camera.getSize().x / 2))
		m_camera.setCenter((m_camera.getSize().x / 2), m_camera.getCenter().y);


	m_levelTransition->update(deltaTime);
	m_healthBar->update(*m_player);
	Notification::instance().update(deltaTime, m_window);
	ConversationBox::instance().update(deltaTime, *this);
	ParticleSystem::instance().update(deltaTime);


	// Update positional sound with player position
	SoundPlayer::instance().update(
		deltaTime,
		sf::Vector2f(m_player->getPosition().x + m_player->getSize().x / 2.f, m_player->getPosition().y + m_player->getSize().y / 2.f));


	//kollision, flytta hj�rna
	for (unsigned int i = 0; i < m_level.getUnits().size(); i++)
	{
		Unit* currentUnit = m_level.getUnits()[i].get();
		if (m_player != currentUnit)
		{
			if (currentUnit->getCollisionRect().intersects(m_player->getCollisionRect()))
			{
				currentUnit->onCollideWith(m_player);
			}
		}
	}
	//If player is dead 
	if (m_player->getHealth() <= 0 && m_player->getSpeed().y==0 && !m_levelTransition->getActive())
	{
		SoundPlayer::instance().playSound("DeathSound", m_player->getPosition());
		changeLevelTransition(m_levelIndex, false);
		m_player->setHealth(-99);
	}
	//if player is outside room
	if (m_player->getPosition().x > (Constants::MapWidth - 1)*Constants::TileSize && !m_levelTransition->getActive())
		changeLevelTransition(m_levelIndex + 1, true);
	if (m_player->getPosition().x < Constants::TileSize*0.5 && !m_levelTransition->getActive())
		changeLevelTransition(m_levelIndex - 1, true);
	if (m_player->getPosition().y >(Constants::MapHeight - 1)*Constants::TileSize && !m_levelTransition->getActive())
	{
		SoundPlayer::instance().playSound("DeathSound", m_player->getPosition());
		changeLevelTransition(m_levelIndex, false);
		m_player->setHealth(-99);
	}
	//Keep player bound whitin room sides
	m_player->setPosition(sf::Vector2f(Utility::clampValue<float>(m_player->getPosition().x, Constants::TileSize*0.1f, (Constants::MapWidth - 0.5f)*Constants::TileSize), m_player->getPosition().y));

}
void Game::draw()
{
	// Draw with normal camera
	m_window.setView(m_camera);
	m_level.draw(m_camera);
	m_inventoryButton->draw();
	m_inventory->draw();
	m_popup->draw();
	m_spiritBar->draw();
	m_healthBar->draw();
	Notification::instance().draw();
	ConversationBox::instance().draw();
	m_levelTransition->draw();
	ParticleSystem::instance().draw();
	Renderer::instance().executeDraws();

	// Draw extra cameras
	for (std::size_t i = 0; i < m_extraCameras.size(); i++)
	{
		m_window.setView(m_extraCameras[i]);
		m_level.draw(m_camera); // Draw only level for custom cameras
		Renderer::instance().executeDraws();
	}

	// Reset to normal view
	m_window.setView(m_camera);

	m_extraCameras.clear();
}
