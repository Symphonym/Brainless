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
#include "Level.h"
#include "FileSave.h"
#include "TileMap.h"
#include "Tile.h"



Game::Game()
:
m_game(sf::VideoMode(1280, 720, sf::Style::Close), "Brainless")
{
	// Set render target to the game
	Renderer::instance().setTarget(m_game);
	m_camera = m_game.getDefaultView();

	// Load game resources
	ResourceLoader::instance().loadFont("Game", "VCR_OSD_MONO.ttf");
	ResourceLoader::instance().loadTexture("TestItem", "pickup.png");
	ResourceLoader::instance().loadTexture("TestItem2", "wizard_idle.png");
	ResourceLoader::instance().loadTexture("testImage", "spritesheet.png");
	ResourceLoader::instance().loadTexture("InventorySlot", "invSlot.png");
	ResourceLoader::instance().loadTexture("PlayerSheet", "Spritesheet_Test_v3_2.png");
	ResourceLoader::instance().loadTexture("PlayerSheetJump", "spritesheet_Skelett_hopp_v1.png");

	//ResourceLoader::instance().loadShader("TestShader", "shaderTest.txt");

	// TODO TEMPORARY, SHOULD NOT BE IN FINAL GAME, prolly put inventory in player class
	m_inventory = new Inventory();

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
	m_player->addTexture(ResourceLoader::instance().retrieveTexture("PlayerSheet"));
	m_player->addTexture(ResourceLoader::instance().retrieveTexture("PlayerSheetJump"));
}
Game::~Game()
{
	delete m_inventory;
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
		const float cameraSpeed = deltaTime*2000.f;
		const float zoomSpeed = deltaTime;

		// Update game logic and input
		m_camera.setCenter(sf::Vector2f(m_player->getPosition().x, m_player->getPosition().y));
		m_player->checkPlayerInput();
		m_level.update(deltaTime);
		m_inventory->update(m_game);

		sf::Event event;
		while (m_game.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_game.close();

			m_inventory->events(event, m_game, m_level);
		}

		// Camera movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			m_camera.move(0, -cameraSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			m_camera.move(0, cameraSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			m_camera.move(-cameraSpeed, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			m_camera.move(cameraSpeed, 0);

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
}

void Game::draw()
{
	m_level.draw(m_camera);
	m_inventory->draw();
	Renderer::instance().executeDraws();
}