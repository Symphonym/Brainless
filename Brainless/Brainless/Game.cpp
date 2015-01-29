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
	m_camera = m_game.getDefaultView();
	Renderer::instance().setTarget(m_game);

	// Load editor resources
	ResourceLoader::instance().loadFont("EditorFont", "VCR_OSD_MONO.ttf");
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


	/*m_gridMode = new EditorGridMode(m_level.getTileMap());
	m_spriteMode = new EditorSpriteMode(m_level.getDecorations());
	m_itemMode = new EditorItemMode(m_level.getItems());*/

	// Initialize save text
	/*m_saveText.setFont(ResourceLoader::instance().retrieveFont("EditorFont"));
	m_saveText.setPosition(0, 0);
	m_saveText.setCharacterSize(16);
	m_saveText.setString("File is saved!");
	m_saveText.setColor(sf::Color::Green);*/
	//Temporary placment marker
	sf::Image markerImg;
	markerImg.create(60, 90, sf::Color::Yellow);

	m_markerTexture.loadFromImage(markerImg);
	m_markerSprite.setTexture(m_markerTexture);

	loadFile();
	player = new Player(Constants::TileSize * 3, Constants::TileSize * 3.4, 60, 90, 300, 300);
	units.push_back(player);
	player->setTexture(ResourceLoader::instance().retrieveTexture("testImage"));
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
		player->checkPlayerInput();

		for (unsigned int i = 0; i < units.size(); i++)
		{
			//Reset marker color
			m_markerSprite.setColor(sf::Color::Color(255, 255, 255, 128));

			float old_x = units[i]->getPositionX();
			float old_y = units[i]->getPositionY();
			units[i]->updateMovement(600, deltaTime);

			//Find colliding boxes
			int box_start_x = floor(units[i]->getPositionX() / Constants::TileSize)-1;
			int box_end_x = ceil((units[i]->getPositionX() + units[i]->getWidth()) / Constants::TileSize)+1;
			int box_start_y = floor(units[i]->getPositionY() / Constants::TileSize);
			int box_end_y = ceil((units[i]->getPositionY() + units[i]->getHeight()) / Constants::TileSize)+1;
			bool airborne = true;

			for (int x = std::max(box_start_x, 0); x < std::min(box_end_x, Constants::MapWidth); x++)
			{
				for (int y = std::max(box_start_y, 0); y < std::min(box_end_y, Constants::MapHeight); y++)
				{
					if (m_level.getTileMap().getTile(x, y).getType() != Tile::Nothing)
					{
						if (m_level.getTileMap().getTile(x, y).collidesWith(units[i]->getCollisionRect()))
						{
							if (std::abs(old_y - (units[i]->getPositionY())) > std::abs(old_x - (units[i]->getPositionX())))
							{
								units[i]->setSpeed(units[i]->getSpeedX(), 0);
								//units[i]->setAcceleration(units[i]->getAccelerationX(), 0);
							}
							else
							{
								//units[i]->setSpeed(0,units[i]->getSpeedY());
							}
							//Move out of contact
							//units[i]->setPosition(units[i]->getPositionX() - units[i]->getSpeedX(), units[i]->getPositionY() - units[i]->getSpeedY());
							units[i]->setPosition(old_x,old_y);
						}

						//Checks if ground is below, makes the player able to jump
						//Kind of a temporary solution, but it works well
						if (m_level.getTileMap().getTile(x, y).collidesWith(sf::FloatRect(units[i]->getPositionX(), units[i]->getPositionY()+1, units[i]->getWidth(), units[i]->getHeight() + 2)))
						{
							airborne = false;
						}

						//Down
						if (m_level.getTileMap().getTile(x, y).collidesWith(sf::FloatRect(units[i]->getPositionX(), units[i]->getPositionY(), units[i]->getWidth(), units[i]->getHeight() + 2)))
						{
							units[i]->setAcceleration(units[i]->getAccelerationX(), 0);
							units[i]->setPosition(units[i]->getPositionX(), y*Constants::TileSize - units[i]->getHeight());
							units[i]->setSpeed(units[i]->getSpeedX(), 0);

							airborne = false;
						}
						//Left && Right
						if ((units[i]->getSpeedX()<0 && m_level.getTileMap().getTile(x, y).collidesWith(sf::FloatRect(units[i]->getPositionX()-2, units[i]->getPositionY(), units[i]->getWidth()+2, units[i]->getHeight()))) ||
							(units[i]->getSpeedX()>0 && m_level.getTileMap().getTile(x, y).collidesWith(sf::FloatRect(units[i]->getPositionX(), units[i]->getPositionY(), units[i]->getWidth()+2, units[i]->getHeight()))) &&
							airborne == true)
						{
							m_markerSprite.setColor(sf::Color::Color(128, 64, 0, 128));
							units[i]->setSpeed(0,units[i]->getSpeedY());
							units[i]->setAcceleration(0,units[i]->getAccelerationY());
							units[i]->setPosition(units[i]->getPositionX()-units[i]->getSpeedX(), units[i]->getPositionY());
						}
						
					}
				}
			}
			units[i]->setStatus(airborne);
			//Move marker to unit
			m_markerSprite.setPosition(units[i]->getPositionX(), units[i]->getPositionY());
		}
		player->updateAnimation(deltaTime);
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
	for (unsigned int i = 0; i < units.size(); i++)
	{
		units[i]->draw();
	}
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