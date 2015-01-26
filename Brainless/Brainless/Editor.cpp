#include "Editor.h"
#include "Renderer.h"
#include "TileMap.h"
#include "Constants.h"
#include "Utility.h"
#include "FileSave.h"
#include "ResourceLoader.h"


Editor::Editor()
:
m_map(),
m_camera(),
m_hudCamera(),
m_currentTile(sf::FloatRect(100, 100, 0, 0), Tile::Ground, sf::Vector2f(Constants::LeftTileOffset, Constants::TopTileOffset)),
m_currentSyncID(0),
m_editor(sf::VideoMode(800, 600, sf::Style::Close), "Brainless Editor")
{
	m_camera = m_editor.getDefaultView();
	Renderer::instance().setTarget(m_editor);

	// Load a default map with nothing but ground tiles
	TileMap::TileMapLayout layout;
	for (int x = 0; x < Constants::MapWidth; x++)
	{
		layout.push_back(std::vector<Tile::TileTypes>());
		for (int y = 0; y < Constants::MapHeight; y++)
			layout[x].push_back(Tile::Ground);
	}

	m_map = MapPtr(new TileMap(layout, Constants::TileSize));

	// Create temporary blank image
	sf::Image highlightImg;
	highlightImg.create(Constants::TileSize, Constants::TileSize, sf::Color::White);

	// Make a blank texture which we can use for highlighting
	m_highlightTexture.loadFromImage(highlightImg);
	m_highlightSprite.setTexture(m_highlightTexture);

	// Scale preview tile
	m_currentTile.getSprite().setScale(0.3f, 0.3f);

	// Load text font
	ResourceLoader::instance().loadFont("DefaultFont", "VCR_OSD_MONO.ttf");
	m_saveText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_saveText.setPosition(0, 0);
	m_saveText.setCharacterSize(16);
	m_saveText.setString("File is not saved!");
	m_saveText.setColor(sf::Color::Red);

	loadFile();

}

void Editor::run()
{
	loop();
}

void Editor::loadFile()
{
	FileSave::loadMap(m_map.get(), 0);
}
void Editor::saveFile()
{
	FileSave::saveMap(m_map.get(), 0);
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

		sf::Event event;
		while (m_editor.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_editor.close();
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				int curIndex = static_cast<int>(m_currentTile.getType());
				curIndex += event.mouseWheel.delta;
				curIndex = Utility::clampValue(curIndex, 0, Constants::BlockTypeCount - 1);
				m_currentTile.setType(static_cast<Tile::TileTypes>(curIndex));
			}
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

		// Save hotkey
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			saveFile();
			m_saveText.setString("File is saved!");
			m_saveText.setColor(sf::Color::Green);
		}

		// Update editor camera
		m_editor.setView(m_camera);

		// Get position of mouse and map it to an index
		sf::Vector2i mouseIndex = m_map->positionToIndex(m_editor.mapPixelToCoords(sf::Mouse::getPosition(m_editor)));

		mouseIndex.x = Utility::clampValue<int>(mouseIndex.x, 0, Constants::MapWidth - 1);
		mouseIndex.y = Utility::clampValue<int>(mouseIndex.y, 0, Constants::MapHeight - 1);

		// Set position of highlight relative to mouse
		m_highlightSprite.setPosition(
			m_map->getTile(mouseIndex.x, mouseIndex.y).getBounds().left,
			m_map->getTile(mouseIndex.x, mouseIndex.y).getBounds().top);

		// Change the properties of a tile with left/right mouseclick
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			m_highlightSprite.setColor(sf::Color::Color(0, 255, 0, 128));
			m_map->getTile(mouseIndex.x, mouseIndex.y).setType(m_currentTile.getType());
			m_saveText.setString("File is not saved!");
			m_saveText.setColor(sf::Color::Red);
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			m_highlightSprite.setColor(sf::Color::Color(255, 0, 0, 128));
			m_map->getTile(mouseIndex.x, mouseIndex.y).setType(Tile::Nothing);
			m_saveText.setString("File is not saved!");
			m_saveText.setColor(sf::Color::Red);
		}
		else
			m_highlightSprite.setColor(sf::Color::Color(255, 255, 255, 128));

		m_editor.clear(sf::Color::Black);
		draw();
		m_editor.display();
	}
}

void Editor::draw()
{
	m_map->draw(m_camera);
	Renderer::instance().drawAbove(m_highlightSprite);
	Renderer::instance().drawHUD(m_currentTile.getSprite());
	Renderer::instance().drawHUD(m_saveText);
	
	Renderer::instance().executeDraws();
}