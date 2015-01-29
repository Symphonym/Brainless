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
	ResourceLoader::instance().loadTexture("testImage", "spritesheet.png");
	//ResourceLoader::instance().loadShader("TestShader", "shaderTest.txt");



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

	m_markerTexture.loadFromImage(markerImg);
	m_markerSprite.setTexture(m_markerTexture);

	loadFile();
	m_player = static_cast<Player*>(m_level.addUnit(Level::UnitPtr(new Player(Constants::TileSize * 3, Constants::TileSize * 3.4, 60, 90, 300, 300))));
	m_player->setTexture(ResourceLoader::instance().retrieveTexture("testImage"));
}
Game::~Game()
{
	//Clear units
}

void Game::run()
{
	loop();
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
	sf::Clock tickClock;
	while (m_game.isOpen())
	{
		m_markerSprite.setColor(sf::Color::Color(255, 255, 255, 128));

		// Get delta time for time based movement
		float deltaTime = tickClock.restart().asSeconds();
		const float cameraSpeed = deltaTime*2000.f;
		const float zoomSpeed = deltaTime;

		//Move units out of collisions

		m_camera.setCenter(m_player->getPositionX(), m_player->getPositionY());
		m_player->checkPlayerInput();

		m_level.update(deltaTime);

		m_markerSprite.setPosition(m_player->getPositionX(), m_player->getPositionY());

		sf::Event event;
		while (m_game.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_game.close();
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
	/*switch (m_editorMode)
	{
	case EditorModes::Grid: m_gridMode->draw(); break;
	case EditorModes::Sprite: m_spriteMode->draw(); break;
	case EditorModes::Item: m_itemMode->draw(); break;
	}
	Renderer::instance().drawHUD(m_saveText);*/
	Renderer::instance().drawAbove(m_markerSprite);
	Renderer::instance().executeDraws();

}