#ifndef INCLUDED_EDITOR_ITEM_MODE
#define INCLUDED_EDITOR_ITEM_MODE

#include <SFML\Graphics.hpp>
#include <memory>
#include <vector>

class Item;
class EditorItemMode
{
public:

	explicit EditorItemMode(std::vector<std::unique_ptr<Item> >& itemVector);

	// Returns true if something changed (so editor knows if it needs to resave)
	bool events(const sf::Event &event, const sf::RenderWindow &editorWindow);
	bool update(float deltaTime, const sf::RenderWindow &editorWindow);
	void draw();

private:

	// Reference to the item list of the level
	std::vector<std::unique_ptr<Item> >& m_items;

	// Extra text that is shown only in the editor (per item) when placing items
	std::vector<sf::Text> m_itemInfo;

	sf::Text m_currentIDText; // Just so you know the ID of the current item
	sf::Text m_currentIndexText, m_currentSyncIDText;
	int m_currentIndex, m_currentSyncID;

	// Preview of the currently selected item
	std::unique_ptr<Item> m_currentItem;

};

#endif