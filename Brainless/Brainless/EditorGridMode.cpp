#include "EditorGridMode.h"
#include "Constants.h"
#include "Utility.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>


EditorGridMode::EditorGridMode(TileMap &tilemap)
:
m_tilemap(tilemap),
m_autotilingEnabled(false),
m_currentTile(sf::FloatRect(100, 100, 0, 0), Tile::Ground, sf::Vector2f(Constants::LeftTileOffset, Constants::TopTileOffset))
{
	// Create temporary blank image
	sf::Image highlightImg;
	highlightImg.create(Constants::TileSize, Constants::TileSize, sf::Color::White);

	// Make a blank texture which we can use for highlighting
	m_highlightTexture.loadFromImage(highlightImg);
	m_highlightSprite.setTexture(m_highlightTexture);

	m_currentTile.getSprite().setScale(0.8f, 0.8f);

	// Initialize index text
	m_indexText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_indexText.setString("X: 0  Y: 0");
	m_indexText.setPosition(10, 200);

	// Initialize autotiling text
	m_autotilingText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_autotilingText.setPosition(10, 500);
	m_autotilingText.setColor(sf::Color::Green);
	m_autotilingText.setString("Autotiling: " + std::string((m_autotilingEnabled ? "enabled" : "disabled")));



	// Add autotiling ranges
	parseAutotilingFile("autotiling/numbersforblocks.txt", "RoadAutotiling");
	//addAutotilingRange("RoadAutotiling", Tile::Road_Middle,
	//{
	//	AutotilingValue(0, Tile::Road_Down)
	//});
}
EditorGridMode::~EditorGridMode()
{

}

bool EditorGridMode::events(const sf::Event &event, const sf::RenderWindow &editorWindow)
{
	if (event.type == sf::Event::MouseWheelMoved)
	{
		// Scroll between block types
		int curIndex = static_cast<int>(m_currentTile.getType());
		curIndex += event.mouseWheel.delta;
		curIndex = Utility::clampValue(curIndex, 0, Constants::BlockTypeCount - 1);

		m_currentTile.setType(static_cast<Tile::TileTypes>(curIndex));
	}

	else if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::M)
		{
			// If Ctrl+M is pressed, refresh autotiling for whole map
			if (event.key.control)
			{
				for (std::size_t x = 0; x < Constants::MapWidth; x++)
				{
					for (std::size_t y = 0; y < Constants::MapHeight; y++)
					{
						Tile &tile = m_tilemap.getTile(x, y);

						auto itr = m_autotiling.find(tile.getAutotilingRangeName());

						// Autotiling range exists
						if (itr != m_autotiling.end())
						{
							int autotilingValue = calculateAutotilingValue(x, y, tile.getAutotilingRangeName());
							m_tilemap.getTile(x, y).setType(itr->second[autotilingValue]);
						}
					}
				}
			}

			// Toggle autotiling
			else
			{
				m_autotilingEnabled = !m_autotilingEnabled;
				m_autotilingText.setString("Autotiling: " + std::string((m_autotilingEnabled ? "enabled" : "disabled")));
			}
			
		}
	}


	return false;
}
bool EditorGridMode::update(float deltaTime, const sf::RenderWindow &editorWindow)
{

	// Get position of mouse and map it to an index
	sf::Vector2i mouseIndex = m_tilemap.positionToIndex(editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow)));

	mouseIndex.x = Utility::clampValue<int>(mouseIndex.x, 0, Constants::MapWidth - 1);
	mouseIndex.y = Utility::clampValue<int>(mouseIndex.y, 0, Constants::MapHeight - 1);

	// Update index text
	m_indexText.setString("X: " + std::to_string(mouseIndex.x) + "  Y: " + std::to_string(mouseIndex.y));

	// Set position of highlight relative to mouse
	m_highlightSprite.setPosition(
		m_tilemap.getTile(mouseIndex.x, mouseIndex.y).getBounds().left,
		m_tilemap.getTile(mouseIndex.x, mouseIndex.y).getBounds().top);

	// Change the properties of a tile with left/right mouseclick
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_highlightSprite.setColor(sf::Color::Color(0, 255, 0, 128));

		// Autotiling is enabled and the current tile has an autotiling range
		if (m_autotilingEnabled && !m_currentTile.getAutotilingRangeName().empty())
		{
			auto itr = m_autotiling.find(m_currentTile.getAutotilingRangeName());

			// Autotiling range exists
			if (itr != m_autotiling.end())
			{
				int autotilingValue = calculateAutotilingValue(mouseIndex.x, mouseIndex.y, m_currentTile.getAutotilingRangeName());
				std::cout << "TILING: " << autotilingValue << std::endl;
				m_tilemap.getTile(mouseIndex.x, mouseIndex.y).setType(itr->second[autotilingValue]);
			}
			else // Just place tile without autotiling
				m_tilemap.getTile(mouseIndex.x, mouseIndex.y).setType(m_currentTile.getType());
		}
		else
			m_tilemap.getTile(mouseIndex.x, mouseIndex.y).setType(m_currentTile.getType());
		return true;
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		m_highlightSprite.setColor(sf::Color::Color(255, 0, 0, 128));
		m_tilemap.getTile(mouseIndex.x, mouseIndex.y).setType(Tile::Nothing);
		return true;
	}
	else
	{
		m_highlightSprite.setColor(sf::Color::Color(255, 255, 255, 128));
		return false;
	}
}
void EditorGridMode::draw()
{
	Renderer::instance().drawAbove(m_highlightSprite);
	Renderer::instance().drawHUD(m_currentTile.getSprite());
	Renderer::instance().drawHUD(m_indexText);
	Renderer::instance().drawHUD(m_autotilingText);
}






void EditorGridMode::addAutotilingRange(const std::string &name, Tile::TileTypes defaultValue, const std::vector<AutotilingValue> &autotilingRange)
{

	AutotilingData data;

	// Initialize with default value
	for (std::size_t i = 0; i < data.size(); i++)
		data[i] = defaultValue;

	// Fill with new autotiling data
	for (auto &autotilingValue : autotilingRange)
		data[autotilingValue.first] = autotilingValue.second;

	// Store the data
	m_autotiling[name] = data;
}
int EditorGridMode::calculateAutotilingValue(int x, int y, const std::string &autotilingRangeName)
{
	int autotilingValue = 0;


	/*
	Autotiling bitmap
	1   2   4
	126 +   8
	64  32 16
	*/

	// Tile to the top left exists
	if (y - 1 >= 0 && x - 1 >= 0 && m_tilemap.getTile(x - 1, y - 1).getAutotilingRangeName() == autotilingRangeName)
		autotilingValue += 1;

	// Tile to the top right exists
	if (y - 1 >= 0 && x + 1 < Constants::MapWidth && m_tilemap.getTile(x + 1, y - 1).getAutotilingRangeName() == autotilingRangeName)
		autotilingValue += 4;

	// Tile to the bottom right exists
	if (y + 1 < Constants::MapHeight && x + 1 < Constants::MapWidth && m_tilemap.getTile(x + 1, y + 1).getAutotilingRangeName() == autotilingRangeName)
		autotilingValue += 16;

	// Tile to the bottom left exists
	if (y + 1 < Constants::MapHeight && x - 1 >= 0 && m_tilemap.getTile(x - 1, y + 1).getAutotilingRangeName() == autotilingRangeName)
		autotilingValue += 64;

	// Tile above exists
	if (y - 1 >= 0 && m_tilemap.getTile(x, y - 1).getAutotilingRangeName() == autotilingRangeName)
		autotilingValue += 2;

	// Tile below exists
	if (y + 1 < Constants::MapHeight && m_tilemap.getTile(x, y + 1).getAutotilingRangeName() == autotilingRangeName)
		autotilingValue += 32;

	// Tile to the left exists
	if (x - 1 >= 0 && m_tilemap.getTile(x - 1, y).getAutotilingRangeName() == autotilingRangeName)
		autotilingValue += 126;

	// Tile to the right exists
	if (x + 1 < Constants::MapWidth && m_tilemap.getTile(x + 1, y).getAutotilingRangeName() == autotilingRangeName)
		autotilingValue += 8;
	
	return autotilingValue;
}
void EditorGridMode::parseAutotilingFile(const std::string &fileName, const std::string &rangeName)
{
	std::ifstream reader(fileName);

	if (reader.is_open())
	{
		// Stores the lines of the file to debug for duplicate autotiling values
		std::unordered_map<int, std::string> duplicateDebug;

		AutotilingData data;

		// Default initialize with something obvious, as all values should be set
		for (std::size_t i = 0; i < data.size(); i++)
			data[i] = Tile::Red;

		std::string line;
		while (std::getline(reader, line))
		{
			std::stringstream ss(line);
			
			// Read tiletype at start of line
			int tileType = 0;
			ss >> tileType;
			Tile::TileTypes tileTypeReal = static_cast<Tile::TileTypes>(tileType);;

			// Read all the autotiling values this tiletype corresponds to
			std::string autotilingValueStr;
			ss >> autotilingValueStr;

			std::vector<std::string> autotilingValues = Utility::splitString(autotilingValueStr, ',');
			for (auto &value : autotilingValues)
			{
				int val = Utility::stringToNumber<std::size_t>(value);
				
				auto dupItr = duplicateDebug.find(val);
				if (dupItr != duplicateDebug.end())
				{
					std::cout << "Duplicate autotiling value: " << val << std::endl;
					std::cout << "Encountered first at: " << dupItr->second << std::endl;
					std::cout << "Encountered again at: " << line << std::endl;
				}
				
				duplicateDebug[val] = line;
				data[val] = tileTypeReal;
			}
		}

		m_autotiling[rangeName] = data;
	}
	reader.close();
}
std::vector<sf::Vector2i> EditorGridMode::getAdjacentTileIndices(int x, int y)
{
	std::vector<sf::Vector2i> result;

	// Tile to the top left exists
	if (y - 1 >= 0 && x - 1 >= 0)
		result.push_back(sf::Vector2i(x - 1, y - 1));

	// Tile to the top right exists
	if (y - 1 >= 0 && x + 1 < Constants::MapWidth)
		result.push_back(sf::Vector2i(x + 1, y - 1));

	// Tile to the bottom right exists
	if (y + 1 < Constants::MapHeight && x + 1 < Constants::MapWidth)
		result.push_back(sf::Vector2i(x + 1, y + 1));

	// Tile to the bottom left exists
	if (y + 1 < Constants::MapHeight && x - 1 >= 0)
		result.push_back(sf::Vector2i(x - 1, y + 1));

	// Tile above exists
	if (y - 1 >= 0)
		result.push_back(sf::Vector2i(x, y - 1));

	// Tile below exists
	if (y + 1 < Constants::MapHeight)
		result.push_back(sf::Vector2i(x, y + 1));

	// Tile to the left exists
	if (x - 1 >= 0)
		result.push_back(sf::Vector2i(x - 1, y));

	// Tile to the right exists
	if (x + 1 < Constants::MapWidth)
		result.push_back(sf::Vector2i(x + 1, y));

	return result;
}