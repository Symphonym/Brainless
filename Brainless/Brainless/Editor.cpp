#include "Editor.h"
#include "Renderer.h"
#include "TileMap.h"
#include "Constants.h"
#include "Utility.h"
#include "FileSave.h"
#include "ResourceLoader.h"
#include "EditorGridMode.h"
#include "EditorSpriteMode.h"
#include "EditorItemMode.h"



Editor::Editor()
:
m_editor(sf::VideoMode(1280, 720, sf::Style::Close), "Brainless Editor"),
m_editorMode(EditorModes::Grid),
m_gridMode(nullptr),
m_spriteMode(nullptr),
m_currentSyncID(0)
{
	m_camera = m_editor.getDefaultView();
	Renderer::instance().setTarget(m_editor);


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


	m_gridMode = new EditorGridMode(m_level.getTileMap());
	m_spriteMode = new EditorSpriteMode(m_level.getDecorations());
	m_itemMode = new EditorItemMode(m_level.getItems());

	// Initialize save text
	m_saveText.setFont(ResourceLoader::instance().retrieveFont("EditorFont"));
	m_saveText.setPosition(0, 0);
	m_saveText.setCharacterSize(16);
	m_saveText.setString("File is saved!");
	m_saveText.setColor(sf::Color::Green);

	loadFile();

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
	FileSave::loadMap(&m_level, 0);
}
void Editor::saveFile()
{
	FileSave::saveMap(&m_level, 0);
}


void Editor::loop()
{
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

			switch (m_editorMode)
			{
				case EditorModes::Grid: 
					somethingChanged = m_gridMode->events(event, m_editor) ? true : somethingChanged;
					break;
				case EditorModes::Sprite:
					somethingChanged = m_spriteMode->events(event, m_editor) ? true : somethingChanged;
					break;
				case EditorModes::Item:
					somethingChanged = m_itemMode->events(event, m_editor) ? true : somethingChanged;
					break;
			}
		}

		// Switch between modes
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
			m_editorMode = EditorModes::Grid;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
			m_editorMode = EditorModes::Sprite;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
			m_editorMode = EditorModes::Item;

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

		// Save hotkey
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			saveFile();
			m_saveText.setString("File is saved!");
			m_saveText.setColor(sf::Color::Green);
		}

		// Update editor camera
		m_editor.setView(m_camera);

		// Update depending on editor mode
		switch (m_editorMode)
		{
			case EditorModes::Grid:
				somethingChanged = m_gridMode->update(deltaTime, m_editor) ? true : somethingChanged;
				break;
			case EditorModes::Sprite:
				somethingChanged = m_spriteMode->update(deltaTime, m_editor) ? true : somethingChanged;
				break;
			case EditorModes::Item:
				somethingChanged = m_itemMode->update(deltaTime, m_editor) ? true : somethingChanged;
				break;
		}

		if (somethingChanged)
		{
			m_saveText.setString("File is not saved!");
			m_saveText.setColor(sf::Color::Red);
		}

		m_editor.clear(sf::Color::Black);
		draw();
		m_editor.display();
	}
}

void Editor::draw()
{
	m_level.draw(m_camera);

	switch (m_editorMode)
	{
		case EditorModes::Grid: m_gridMode->draw(); break;
		case EditorModes::Sprite: m_spriteMode->draw(); break;
		case EditorModes::Item: m_itemMode->draw(); break;
	}
	Renderer::instance().drawHUD(m_saveText);
	
	Renderer::instance().executeDraws();
}
