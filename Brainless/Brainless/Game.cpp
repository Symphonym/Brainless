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
m_game(sf::VideoMode(1280, 720, sf::Style::Close), "Brainless")
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
		Notification::instance().setPosition(itm->getPosition());
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
				itm->onUse(m_game);

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

	loadFile();
	m_player = static_cast<Player*>(m_level.addUnit(Level::UnitPtr(new Player(sf::Vector2f(Constants::TileSize * 3, Constants::TileSize * 3.4)))));

	//temp, texture borde laddas in på annat sätt.
	Unit* temp = new WalkingZombie(sf::Vector2f(Constants::TileSize * 9, Constants::TileSize * 3), 100);
	temp->addTexture(ResourceLoader::instance().retrieveTexture("Zombie"));
	m_level.addUnit(Level::UnitPtr(temp));
	temp = new IdleZombie(sf::Vector2f(Constants::TileSize * 8, Constants::TileSize * 3));
	temp->addTexture(ResourceLoader::instance().retrieveTexture("Zombie"));
	m_level.addUnit(Level::UnitPtr(temp));
	m_player->addTexture(ResourceLoader::instance().retrieveTexture("PlayerSheet"));
	m_player->addTexture(ResourceLoader::instance().retrieveTexture("PlayerSheetJump"));
}
Game::~Game()
{
	delete m_inventory;
	delete m_popup;
	//Clear units
}

void Game::run()
{
	loop();
}

void Game::loadFile()
{
	FileSave::loadMapText(m_level, 0);
}
void Game::saveFile()
{
	FileSave::loadMapText(m_level, 0);
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
			if (event.type == sf::Event::KeyReleased)
			{
				sf::Vector2f mousePos = m_game.mapPixelToCoords(sf::Mouse::getPosition(m_game));
				SoundPlayer::instance().playSound("CoinTestSound", mousePos);
			}

			m_inventory->events(event, m_game, m_level);
			m_popup->events(event, m_game, m_level);
			ConversationBox::instance().events(event, m_game);
		}
		
		//Pause if out of focus
		if (m_game.hasFocus())
		{
			m_game.setActive(true);
			// Update game logic and input
			m_camera.setCenter(sf::Vector2f(m_player->getPosition().x, m_player->getPosition().y));
		//	m_player->checkPlayerInput(deltaTime);
			m_level.update(deltaTime);
			m_inventory->update(m_game);
			Notification::instance().update(deltaTime);
			ConversationBox::instance().update(deltaTime, m_game);
		//kollision, flytta hjärna
			for (unsigned int i = 0; i < m_level.getUnits().size(); i++)
			{
				Unit* currentUnit = m_level.getUnits()[i].get();
				if (m_player != currentUnit)
				{
					if (currentUnit->getCollisionRect().intersects(m_player->getCollisionRect()))
					{
						std::cout << "JAG DOG" << std::endl;
					}
				}
			}
			/*
			 for(alla units)
			 {
				 if(m_player != currentUnit)
				 {
					if(collision)
					{
						spelaren Dööööörr dramatiskt
					}
				 }
			 }
			*/
			m_popup->update(m_game, m_player->getPosition());
	

		SoundPlayer::instance().update(
			deltaTime,
			sf::Vector2f(m_player->getPosition().x + m_player->getSize().x / 2.f, m_player->getPosition().y + m_player->getSize().y / 2.f));

		// Camera zoom
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			m_camera.zoom(1.f + zoomSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			m_camera.zoom(1.f - zoomSpeed);

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