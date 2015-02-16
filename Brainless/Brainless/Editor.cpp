#include "Editor.h"
#include "Renderer.h"
#include "TileMap.h"
#include "Constants.h"
#include "Utility.h"
#include "FileSave.h"
#include "ResourceLoader.h"
#include "EditorGridMode.h"
#include "EditorSpriteMode.h"
#include "EditorZombieMode.h"
#include "EditorItemMode.h"
#include "Button.h"



Editor::Editor()
:
m_editor(sf::VideoMode(1280, 720, sf::Style::Close), "Brainless Editor"),
m_currentLevelFileIndex(0),
m_editorMode(EditorModes::Grid),
m_gridMode(nullptr),
m_spriteMode(nullptr),
m_currentSyncID(0),
m_isMenu(false)
{
	m_camera = m_editor.getDefaultView();
	Renderer::instance().setTarget(m_editor);


	// Load editor resources
	ResourceLoader::instance().loadResourceFile("loadfiles/ResourceLoad_Editor.txt");

	//Renderer::instance().plugShader(ResourceLoader::instance().retrieveShader("BlackAndWhiteShader"));
	//Renderer::instance().plugShader(ResourceLoader::instance().retrieveShader("DarknessShader"));
	//ResourceLoader::instance().loadShader("TestShader", "shaderTest.txt");

	m_editorBackground.setPosition(0, -40);
	m_editorBackground.setTexture(ResourceLoader::instance().retrieveTexture("EditorLevelSize"));

	m_spawnSprite.setTexture(ResourceLoader::instance().retrieveTexture("PlayerSpawnItem"));
	m_spawnSprite.setOrigin(m_spawnSprite.getGlobalBounds().width / 2.f, m_spawnSprite.getGlobalBounds().height / 2.f);
	m_spawnSprite.setPosition(300, 300);

	// Load a default map with nothing but ground tiles
	TileMap::TileMapLayout layout;
	for (int x = 0; x < Constants::MapWidth; x++)
	{
		layout.push_back(std::vector<Tile::TileTypes>());
		for (int y = 0; y < Constants::MapHeight; y++)
			layout[x].push_back(Tile::Ground);
	}

	// TODO TEST CODE DONT REMOVE
	shaderTest = 0;

	// Initialize the different editor modes
	m_gridMode = new EditorGridMode(m_level.getTileMap());
	m_spriteMode = new EditorSpriteMode();
	m_zombieMode = new EditorZombieMode();
	m_itemMode = new EditorItemMode();

	// Load the file and reload data for all the modes
	loadFile();



	// Initialize save text
	m_saveText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_saveText.setPosition(0, 0);
	m_saveText.setCharacterSize(16);
	m_saveText.setString("File is saved!");
	m_saveText.setColor(sf::Color::Green);

	// Initialize level file text
	m_levelFileText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_levelFileText.setPosition(200, 0);
	m_levelFileText.setColor(sf::Color::Green);
	m_levelFileText.setString("Editing level: level" + std::to_string(m_currentLevelFileIndex) + ".txt");
}
Editor::~Editor()
{
	delete m_gridMode;
	delete m_spriteMode;
	delete m_itemMode;
}

void Editor::run()
{
	loop();
}

void Editor::loadFile()
{
	// Reset the level and load the new level
	m_level.reset();
	FileSave::loadMapText(m_level, m_currentLevelFileIndex);

	m_spawnSprite.setPosition(m_level.getSpawnPos());

	// Reload info for different modes
	m_itemMode->reloadDebugText(m_level);
}
void Editor::saveFile()
{
	//FileSave::saveMap(&m_level, 0);
	FileSave::saveMapText(m_level, m_currentLevelFileIndex);
	m_saveText.setString("File is saved!");
	m_saveText.setColor(sf::Color::Green);
}


void Editor::loop()
{
	// Buttons
	Button tileButton(ResourceLoader::instance().retrieveTexture("TileButton"), ResourceLoader::instance().retrieveTexture("TileButtonPressed"), sf::IntRect(20, 610, 80, 80), &m_editor);
	Button spriteButton(ResourceLoader::instance().retrieveTexture("SpriteButton"), ResourceLoader::instance().retrieveTexture("SpriteButtonPressed"), sf::IntRect(120, 610, 80, 80), &m_editor);
	Button itemButton(ResourceLoader::instance().retrieveTexture("ItemButton"), ResourceLoader::instance().retrieveTexture("ItemButtonPressed"), sf::IntRect(220, 610, 80, 80), &m_editor);
	Button cameraButton(ResourceLoader::instance().retrieveTexture("CameraButton"), ResourceLoader::instance().retrieveTexture("CameraButtonPressed"), sf::IntRect(320, 610, 80, 80), &m_editor);
	Button saveButton(ResourceLoader::instance().retrieveTexture("SaveButton"), ResourceLoader::instance().retrieveTexture("SaveButtonPressed"), sf::IntRect(420, 610, 80, 80), &m_editor);
	m_isMenu = false;

	sf::Clock tickClock;
	while (m_editor.isOpen())
	{
		// Get delta time for time based movement
		float deltaTime = tickClock.restart().asSeconds();
		const float cameraSpeed = deltaTime*2000.f;
		const float zoomSpeed = deltaTime;

		// If something was modified in the level
		bool somethingChanged = false;

		sf::Event event;
		while (m_editor.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_editor.close();

			if (event.type == sf::Event::KeyReleased)
			{
				// Toggle menu
				if (event.key.code == sf::Keyboard::Return)
					m_isMenu = !m_isMenu;
				// Save file
				else if (event.key.code == sf::Keyboard::S && event.key.control)
					saveFile();
				else if (event.key.code == sf::Keyboard::F)
				{
					m_spawnSprite.setPosition(m_editor.mapPixelToCoords(sf::Mouse::getPosition(m_editor)));
					m_level.setSpawnPosition(m_spawnSprite.getPosition());
					somethingChanged = true;
				}

				// Go down in levels
				else if (event.key.code == sf::Keyboard::Y)
				{
					++m_currentLevelFileIndex;
					m_currentLevelFileIndex = Utility::clampValue<int>(m_currentLevelFileIndex, 0, INT_MAX);

					m_levelFileText.setString("Editing level: level" + std::to_string(m_currentLevelFileIndex) + ".txt");
					loadFile();
				}

				// Go up in levels
				else if (event.key.code == sf::Keyboard::U)
				{
					--m_currentLevelFileIndex;
					m_currentLevelFileIndex = Utility::clampValue<int>(m_currentLevelFileIndex, 0, INT_MAX);

					m_levelFileText.setString("Editing level: level" + std::to_string(m_currentLevelFileIndex) + ".txt");
					loadFile();
				}
			}

			if (!m_isMenu)
			{
				switch (m_editorMode)
				{
				case EditorModes::Grid:
					somethingChanged = m_gridMode->events(event, m_editor) ? true : somethingChanged;
					break;
				case EditorModes::Sprite:
					somethingChanged = m_spriteMode->events(event, m_editor, m_level) ? true : somethingChanged;
					break;
				case EditorModes::Zombie:
					somethingChanged = m_zombieMode->events(event, m_editor, m_level) ? true : somethingChanged;
					break;
				case EditorModes::Item:
					somethingChanged = m_itemMode->events(event, m_editor, m_level) ? true : somethingChanged;
					break;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////// SHADER TEST CODE
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			shaderTest += deltaTime;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			shaderTest -= deltaTime;

		shaderTest = Utility::clampValue<float>(shaderTest, 0, 1);
		//sf::Shader &shader = ResourceLoader::instance().retrieveShader("BlackAndWhiteShader");
		//shader.setParameter("intensityValue", shaderTest);
		//shader.setParameter("image", sf::Shader::CurrentTexture);
		//sf::Shader &shader = ResourceLoader::instance().retrieveShader("DarknessShader");
		//shader.setParameter("playerScreenPosition", m_editor.getSize().x/2, m_editor.getSize().y / 2);
		//shader.setParameter("lightReachDistance", 300);

		//shader.setParameter("mousePos", sf::Mouse::getPosition(m_editor).x, m_editor.getSize().y - sf::Mouse::getPosition(m_editor).y);
		//////////////////////////////////////////////////////////////////////////// SHADER TEST CODE END

		if (m_isMenu)
		{
			// Switch between modes
			if (tileButton.getReleased() || sf::Keyboard::isKeyPressed(sf::Keyboard::J))
			{
				m_editorMode = EditorModes::Grid;
			}
			else if (spriteButton.getReleased() || sf::Keyboard::isKeyPressed(sf::Keyboard::K))
			{
				m_editorMode = EditorModes::Sprite;
			}
			else if (itemButton.getReleased() || sf::Keyboard::isKeyPressed(sf::Keyboard::L))
			{
				m_editorMode = EditorModes::Item;
			}
			else if (cameraButton.getReleased())
			{
				sf::Vector2f curCenter = m_camera.getCenter();
				m_camera = m_editor.getDefaultView();
				m_camera.setCenter(curCenter);
			}
			else if (saveButton.getReleased())
			{
				saveFile();
				m_saveText.setString("File is saved!");
				m_saveText.setColor(sf::Color::Green);
			}
		}

		// Switch between modes
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
		{
			m_editorMode = EditorModes::Grid;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
		{
			m_editorMode = EditorModes::Sprite;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
		{
			m_editorMode = EditorModes::Zombie;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		{
			m_editorMode = EditorModes::Item;
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

		// Camera zoom reset
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			sf::Vector2f curCenter = m_camera.getCenter();
			m_camera = m_editor.getDefaultView();
			m_camera.setCenter(curCenter);

		}

		// Update editor camera
		m_editor.setView(m_camera);

		if (!m_isMenu)
		{
			// Update depending on editor mode
			switch (m_editorMode)
			{
			case EditorModes::Grid:
				somethingChanged = m_gridMode->update(deltaTime, m_editor) ? true : somethingChanged;
				break;
			case EditorModes::Sprite:
				somethingChanged = m_spriteMode->update(deltaTime, m_editor) ? true : somethingChanged;
				break;
			case EditorModes::Zombie:
				somethingChanged = m_zombieMode->update(deltaTime, m_editor) ? true : somethingChanged;
				break;
			case EditorModes::Item:
				somethingChanged = m_itemMode->update(deltaTime, m_editor) ? true : somethingChanged;
				break;
			}
		}

		if (somethingChanged)
		{
			m_saveText.setString("File is not saved!");
			m_saveText.setColor(sf::Color::Red);
		}

		m_editor.clear(sf::Color::Black);
		draw();
		m_editor.display();

		if (m_isMenu)
		{
			tileButton.draw();
			spriteButton.draw();
			itemButton.draw();
			cameraButton.draw();
			saveButton.draw();
		}
	}
}

void Editor::draw()
{
	Renderer::instance().drawBackground(m_editorBackground);
	m_level.draw(m_camera);

	switch (m_editorMode)
	{
	case EditorModes::Grid: m_gridMode->draw(); break;
	case EditorModes::Sprite: m_spriteMode->draw(); break;
	case EditorModes::Zombie: m_zombieMode->draw(); break;
	case EditorModes::Item: m_itemMode->draw(); break;
	}
	Renderer::instance().drawHUD(m_saveText);
	Renderer::instance().drawHUD(m_levelFileText);
	Renderer::instance().drawAbove(m_spawnSprite);

	Renderer::instance().executeDraws();
}
