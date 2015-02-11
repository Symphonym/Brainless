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

Game::Game(StateMachine &machine)
:
State(machine),
m_levelIndex(0)
{
	m_camera = m_window.getDefaultView();

	// Load game resources
	ResourceLoader::instance().loadFromFile("loadfiles/ResourceLoad_Game.txt");


	// TODO TEMPORARY, SHOULD NOT BE IN FINAL GAME, prolly put inventory in player class
	m_inventory = new Inventory();
	m_popup = new PopUpMenu();
	m_popup->setItemCallback([&](Item* itm, PopUpMenu::InteractTypes type) -> void
	{
		// Can't do anything if you're talking to someone
		if (ConversationBox::instance().isShown())
			return;

		if (type == PopUpMenu::InteractTypes::Pickup)
		{
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
			if (itm->isUsable())
			{
				itm->onUse(*this);

				// Sync use functionality for sync
				for (std::size_t i = 0; i < m_level.getItems().size(); i++)
				{
					if (m_level.getItems()[i]->getSyncID() == itm->getSyncID())
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

	// Load a default map with nothing but ground tiles
	TileMap::TileMapLayout layout;
	for (int x = 0; x < Constants::MapWidth; x++)
	{
		layout.push_back(std::vector<Tile::TileTypes>());
		for (int y = 0; y < Constants::MapHeight; y++)
			layout[x].push_back(Tile::Ground);
	}

	//Temporary placment marker
	sf::Image markerImg;
	markerImg.create(60, 90, sf::Color::Yellow);

	m_player = static_cast<Player*>(m_level.addUnit(Level::UnitPtr(new Player(sf::Vector2f(Constants::TileSize * 10, Constants::TileSize * 8)))));

	// TEST CODE FOR LOADING DEFAULT LEVEL
	changeLevel(0);


	//temp, texture borde laddas in på annat sätt.
	m_player->addTexture(ResourceLoader::instance().retrieveTexture("PlayerSheet"));
	m_player->addTexture(ResourceLoader::instance().retrieveTexture("PlayerSheetJump"));
	m_player->addTexture(ResourceLoader::instance().retrieveTexture("PlayerSheetRun"));
}
Game::~Game()
{
	delete m_inventory;
	delete m_popup;
	//Clear units
}

void Game::lootItem(Inventory::ItemPtr item)
{
	m_inventory->addItem(std::move(item));
}

void Game::changeLevel(int levelIndex)
{
	m_levelIndex = levelIndex;
	FileSave::loadMapText(m_level, m_levelIndex);
	m_player->setPosition(m_level.getSpawnPos());
	FileSave::loadLevelProgress(m_level, m_levelIndex);
	FileSave::loadInventory(*m_inventory);
}
void Game::addCamera(const sf::View &camera)
{
	m_extraCameras.push_back(camera);
}

void Game::saveGame()
{
	FileSave::saveInventory(*m_inventory);
	FileSave::saveLevelProgress(m_level, m_levelIndex);
	Notification::instance().write("Game saved successfully!");
}

Player& Game::getPlayer()
{
	return *m_player;
}
Level& Game::getLevel()
{
	return m_level;
}

void Game::events(const sf::Event &event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::N)
			saveGame();
		else if (event.key.code == sf::Keyboard::Escape)
			m_machine.pushState<PauseMenu>();
	}

	// Pump events to everything that needs it
	// Disable game input when conversation is ongoing
	if (!ConversationBox::instance().isShown())
	{
		m_inventory->events(event, m_window, m_level);
		m_popup->events(event, *this);
	}
	else
		ConversationBox::instance().events(event, *this);
}
void Game::update(float deltaTime)
{
	if (m_player->getCameraPosition().x < Constants().MapWidth * Constants().TileSize - (m_camera.getSize().x / 2) && m_player->getCameraPosition().x > 0 + (m_camera.getSize().x / 2))
	m_camera.setCenter(m_player->getCameraPosition().x, m_camera.getCenter().y);

	if (m_player->getCameraPosition().y < Constants().MapHeight * Constants().TileSize - (m_camera.getSize().y / 2) && m_player->getCameraPosition().y > 0 + (m_camera.getSize().y / 2))
		m_camera.setCenter(m_camera.getCenter().x, m_player->getCameraPosition().y);

	// Update game logic and input, if not paused
	// Disable game input when conversation is ongoing
	if (!ConversationBox::instance().isShown())
	{
		m_level.update(deltaTime, *this);
		m_inventory->update(deltaTime, *this);
		m_popup->update(*this,
			sf::Vector2f(m_player->getPosition().x + m_player->getSize().x / 2.f, m_player->getPosition().y + m_player->getSize().y / 2.f));
	}
	Notification::instance().update(deltaTime, m_window);
	ConversationBox::instance().update(deltaTime, *this);

	// Update positional sound with player position
	SoundPlayer::instance().update(
		deltaTime,
		sf::Vector2f(m_player->getPosition().x + m_player->getSize().x / 2.f, m_player->getPosition().y + m_player->getSize().y / 2.f));


	//kollision, flytta hjärna
	for (unsigned int i = 0; i < m_level.getUnits().size(); i++)
	{
		Unit* currentUnit = m_level.getUnits()[i].get();
		if (m_player != currentUnit)
		{
			if (currentUnit->getCollisionRect().intersects(m_player->getCollisionRect()))
			{
				m_player->takesDamage(currentUnit->getPosition() - m_player->getPosition());
			}
		}
	}
}
void Game::draw()
{
	// Draw with normal camera
	m_window.setView(m_camera);
	m_level.draw(m_camera);
	m_inventory->draw();
	m_popup->draw();
	Notification::instance().draw();
	ConversationBox::instance().draw();
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
