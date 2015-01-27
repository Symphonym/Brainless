#include "EditorGridMode.h"
#include "Constants.h"
#include "Utility.h"
#include "Renderer.h"

EditorGridMode::EditorGridMode(TileMap &tilemap)
:
m_tilemap(tilemap),
m_currentTile(sf::FloatRect(100, 100, 0, 0), Tile::Ground, sf::Vector2f(Constants::LeftTileOffset, Constants::TopTileOffset))
{
	// Create temporary blank image
	sf::Image highlightImg;
	highlightImg.create(Constants::TileSize, Constants::TileSize, sf::Color::White);

	// Make a blank texture which we can use for highlighting
	m_highlightTexture.loadFromImage(highlightImg);
	m_highlightSprite.setTexture(m_highlightTexture);

	m_currentTile.getSprite().setScale(0.3f, 0.3f);
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

	return false;
}

bool EditorGridMode::update(float deltaTime, const sf::RenderWindow &editorWindow)
{

	// Get position of mouse and map it to an index
	sf::Vector2i mouseIndex = m_tilemap.positionToIndex(editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow)));

	mouseIndex.x = Utility::clampValue<int>(mouseIndex.x, 0, Constants::MapWidth - 1);
	mouseIndex.y = Utility::clampValue<int>(mouseIndex.y, 0, Constants::MapHeight - 1);

	// Set position of highlight relative to mouse
	m_highlightSprite.setPosition(
		m_tilemap.getTile(mouseIndex.x, mouseIndex.y).getBounds().left,
		m_tilemap.getTile(mouseIndex.x, mouseIndex.y).getBounds().top);

	// Change the properties of a tile with left/right mouseclick
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_highlightSprite.setColor(sf::Color::Color(0, 255, 0, 128));
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
}