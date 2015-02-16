#ifndef INCLUDED_EDITOR_H
#define INCLUDED_EDITOR_H

#include <SFML\Graphics.hpp>
#include <memory>
#include "Tile.h"
#include "Level.h"

class TileMap;
class EditorGridMode;
class EditorSpriteMode;
class EditorZombieMode;
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
		Zombie,
		Item
	};

	// TODO TEST CODE, DONT REMOVE UNLESS YOU MADE THIS
	float shaderTest;


	typedef std::unique_ptr<TileMap> MapPtr;

	sf::RenderWindow m_editor;
	sf::View m_camera;

	Level m_level;
	sf::Sprite m_editorBackground;
	
	// Spawn position sprite
	sf::Sprite m_spawnSprite;

	// Display info
	sf::Text m_saveText;
	sf::Text m_levelFileText;
	int m_currentLevelFileIndex;

	EditorModes m_editorMode;
	EditorGridMode *m_gridMode;
	EditorSpriteMode *m_spriteMode;
	EditorZombieMode *m_zombieMode;
	EditorItemMode *m_itemMode;

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