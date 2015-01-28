#include "Game.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Constants.h"
#include "Utility.h"
#include "Unit.h"
#include "Level.h"
#include "FileSave.h"
#include "TileMap.h"
#include "Tile.h"



Game::Game()
	:
	m_game(sf::VideoMode(1280, 720, sf::Style::Close), "Brainless Editor")
{
	m_camera = m_game.getDefaultView();
	Renderer::instance().setTarget(m_game);

	// Load editor resources
	ResourceLoader::instance().loadFont("EditorFont", "VCR_OSD_MONO.ttf");
	ResourceLoader::instance().loadTexture("TestItem", "pickup.png");
	//ResourceLoader::instance().loadShader("TestShader", "shaderTest.txt");



	// Load a default map with nothing but ground tiles
	TileMap::TileMapLayout layout;
	for (int x = 0; x < Constants::MapWidth; x++)
	{
		layout.push_back(std::vector<Tile::TileTypes>());
		for (int y = 0; y < Constants::MapHeight; y++)
			layout[x].push_back(Tile::Ground);
	}


	/*m_gridMode = new EditorGridMode(m_level.getTileMap());
	m_spriteMode = new EditorSpriteMode(m_level.getDecorations());
	m_itemMode = new EditorItemMode(m_level.getItems());*/

	// Initialize save text
	/*m_saveText.setFont(ResourceLoader::instance().retrieveFont("EditorFont"));
	m_saveText.setPosition(0, 0);
	m_saveText.setCharacterSize(16);
	m_saveText.setString("File is saved!");
	m_saveText.setColor(sf::Color::Green);*/

	loadFile();
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
		// Get delta time for time based movement
		float deltaTime = tickClock.restart().asSeconds();
		const float cameraSpeed = deltaTime*2000.f;
		const float zoomSpeed = deltaTime;

		//Move units out of collisions
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
			for (float x = box_start_x; x < box_end_x; x++)
			{
				for (float y = box_start_y; y < box_end_y; y++)
				{
					//Check for collision
					if (m_level.getTileMap().getTile(x, y).collidesWith(units[i]->getCollisionRect()))
					{
						float unit_x = units[i]->getPositionX(), unit_y = units[i]->getPositionY();
						while (m_level.getTileMap().getTile(x, y).collidesWith(units[i]->getCollisionRect()))
						{
							//Find direction to tile
							//Move in opposite direction tile
							units[i]->setPosition(unit_x, unit_y - 0.1);
						}
						//Move out of contact
						units[i]->setStatus(false);
					}
					else
					{
						units[i]->setStatus(true);
					}
				}
			}
		}

		sf::Event event;
		while (m_game.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_game.close();
		}

		// Camera movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			m_camera.move(0, -cameraSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			m_camera.move(0, cameraSpeed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			m_camera.move(-cameraSpeed, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
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

	Renderer::instance().executeDraws();
}