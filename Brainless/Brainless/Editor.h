#ifndef INCLUDED_EDITOR_H
#define INCLUDED_EDITOR_H

#include <SFML\Graphics.hpp>
#include <memory>
#include "Tile.h"
#include "Level.h"

class TileMap;
class EditorGridMode;
class EditorSpriteMode;
class EditorItemMode;
class Editor
{
public:

	Editor();
	~Editor();

	// Starts the engine
	void run();

private:

	enum EditorModes
	{
		Grid,
		Sprite,
		Item
	};

	// TODO TEST CODE, DONT REMOVE UNLESS YOU MADE THIS
	float shaderTest;

	typedef std::unique_ptr<TileMap> MapPtr;

	sf::RenderWindow m_editor;
	sf::View m_camera;

	Level m_level;
	sf::Sprite m_editorBackground;

	sf::Text m_levelFileText;
	int m_currentLevelFileIndex;

	EditorModes m_editorMode;
	EditorGridMode *m_gridMode;
	EditorSpriteMode *m_spriteMode;
	EditorItemMode *m_itemMode;

	// Whether or not you have saved
	sf::Text m_saveText;

	// Which sync ID you are using when placing an item, used to connect items
	// to eachother. A key and the door it goes to would have a matching syncID
	int m_currentSyncID;

	bool m_isMenu;

	// Load/save functionality
	void loadFile();
	void saveFile();

	// Update loop
	void loop();

	// Rendering function
	void draw();
};

#endif