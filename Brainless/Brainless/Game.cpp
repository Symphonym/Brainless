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
#include "SoundPlayer.h"
#include "Notification.h"
#include "ConversationBox.h"

Game::Game()
:
m_game(sf::VideoMode(1280, 720, sf::Style::Close), "Brainless"),
m_isPaused(false),
m_levelIndex(0)
{
	// Set render target to the game
	Renderer::instance().setTarget(m_game);
	m_camera = m_game.getDefaultView();

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

void Game::setPaused(bool paused)
{
	m_isPaused = paused;
}

void Game::changeLevel(int levelIndex)
{
	m_levelIndex = levelIndex;
	FileSave::loadMapText(m_level, m_levelIndex);
	m_player->setPosition(m_level.getSpawnPos());
	FileSave::loadLevelProgress(m_level, m_levelIndex);
	FileSave::loadInventory(*m_inventory);
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
const sf::RenderWindow& Game::getWindow() const
{
	return m_game;
}

void Game::run()
{
	loop();
}

void Game::loop()
{
	sf::Clock tickClock;
	while (m_game.isOpen())
	{

		// Get delta time for time based movement
		float deltaTime = tickClock.restart().asSeconds();
		const float zoomSpeed = deltaTime;


		sf::Event event;
		while (m_game.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_game.close();
			else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::N)
				saveGame();

			// Pump events to everything that needs it, if not paused
			if (!m_isPaused)
			{
				// Disable game input when conversation is ongoing
				if (!ConversationBox::instance().isShown())
				{
					m_inventory->events(event, m_game, m_level);
					m_popup->events(event, m_game, m_level);
				}
				else
					ConversationBox::instance().events(event, *this);
			}

		}
		
		//Pause if out of focus
		if (m_game.hasFocus())
		{
			m_game.setActive(true);
			m_camera.setCenter(m_player->getCameraPosition());

			// Update game logic and input, if not paused
			if (!m_isPaused)
			{
				// Disable game input when conversation is ongoing
				if (!ConversationBox::instance().isShown())
				{
					m_level.update(deltaTime, *this);
					m_inventory->update(m_game);
					m_popup->update(m_game,
						sf::Vector2f(m_player->getPosition().x + m_player->getSize().x / 2.f, m_player->getPosition().y + m_player->getSize().y / 2.f));
				}
				Notification::instance().update(deltaTime, m_game);
				ConversationBox::instance().update(deltaTime, *this);
			}

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


			// Update editor camera
			m_game.setView(m_camera);

			m_game.clear(sf::Color::Black);
			draw();
			m_game.display();
		}
		else
			m_game.setActive(false);

	}
}

void Game::draw()
{
	m_level.draw(m_camera);
	m_inventory->draw();
	m_popup->draw();
	Notification::instance().draw();
	ConversationBox::instance().draw();
	Renderer::instance().executeDraws();
}