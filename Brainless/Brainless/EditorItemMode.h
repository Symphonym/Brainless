#ifndef INCLUDED_EDITOR_ITEM_MODE
#define INCLUDED_EDITOR_ITEM_MODE

#include <SFML\Graphics.hpp>
#include <memory>
#include <vector>
#include "Level.h"

class Item;
class EditorItemMode
{
public:

	explicit EditorItemMode();

	// Reset all extra debug text and load it again
	void reloadDebugText(Level &level);

	// Returns true if something changed (so editor knows if it needs to resave)
	bool events(const sf::Event &event, const sf::RenderWindow &editorWindow, Level &level);
	bool update(float deltaTime, const sf::RenderWindow &editorWindow);
	void draw();

private:

	// Extra text that is shown only in the editor (per item) when placing items
	std::vector<sf::Text> m_itemInfo;
	//std::vector<sf::VertexArray> m_itemBounds;
	std::vector<sf::RectangleShape> m_itemInteractBounds;
	std::vector<sf::RectangleShape> m_itemCollisionBounds;

	sf::Texture m_infoBackground;
	sf::Sprite m_bgSprite;

	sf::Text m_currentNameText;
	sf::Text m_currentIDText; // Just so you know the ID of the current item
	sf::Text m_currentIndexText, m_currentSyncIDText;
	int m_currentIndex, m_currentSyncID;

	// Preview of the currently selected item
	std::unique_ptr<Item> m_currentItem;

};

#endif