#include "EditorItemMode.h"
#include "Item.h"
#include "ResourceLoader.h"
#include "ItemDatabse.h"
#include "Utility.h"
#include "Renderer.h"

EditorItemMode::EditorItemMode()
:
m_currentIndex(1),
m_currentSyncID(-1)
{
	m_currentIndexText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_currentSyncIDText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_currentIDText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));

	m_currentIndexText.setString("Item index: " + std::to_string(m_currentIndex));
	m_currentSyncIDText.setString("Sync ID: " + std::to_string(m_currentSyncID));

	// Initialize itemID text
	m_currentIDText.setPosition(10, 50);
	m_currentIndexText.setPosition(10, 100);
	m_currentSyncIDText.setPosition(10, 150);

	// Initialize current item and the starting text of currentID with that item
	m_currentItem = std::move(ItemDatabase::instance().extractItemByCount(m_currentIndex));
	m_currentIDText.setString("Item ID: " + std::to_string(m_currentItem->getID()));
}


void EditorItemMode::reloadDebugText(Level &level)
{
	m_itemInfo.clear();

	// Load debug strings for any items already loaded
	for (std::size_t i = 0; i < level.getItems().size(); i++)
	{
		Item& curItem = *level.getItems()[i].get();

		sf::Text text;
		text.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
		text.setString("ID: " + std::to_string(curItem.getID()) + " SyncID: " + std::to_string(curItem.getSyncID()));

		sf::Vector2f textPos(
			curItem.getSprite().getPosition().x + curItem.getSprite().getGlobalBounds().width / 2.f - text.getGlobalBounds().width / 2.f,
			curItem.getSprite().getPosition().y - curItem.getSprite().getGlobalBounds().height / 2.f);
		text.setPosition(textPos);
		m_itemInfo.push_back(text);
	}
}

bool EditorItemMode::events(const sf::Event &event, const sf::RenderWindow &editorWindow, Level &level)
{
	if (event.type == sf::Event::MouseWheelMoved)
	{
		// Scroll between items
		m_currentIndex += event.mouseWheel.delta;
		m_currentIndex = Utility::clampValue<int>(m_currentIndex, 1, ItemDatabase::instance().getItemCount());

		// Update preview item
		m_currentItem = std::move(ItemDatabase::instance().extractItemByCount(m_currentIndex));

		m_currentIDText.setString("Item ID: " + std::to_string(m_currentItem->getID()));
		m_currentIndexText.setString("Item index: " + std::to_string(m_currentIndex));
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		// Place an item
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			// Clone the currently selected item
			std::unique_ptr<Item> newItem(m_currentItem->clone());
			newItem->setSyncID(m_currentSyncID);

			sf::Text text;
			text.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
			text.setString("ID: " + std::to_string(newItem->getID()) + " SyncID: " + std::to_string(newItem->getSyncID()));

			sf::Vector2f textPos(
				newItem->getSprite().getPosition().x + newItem->getSprite().getGlobalBounds().width / 2.f - text.getGlobalBounds().width / 2.f,
				newItem->getSprite().getPosition().y - newItem->getSprite().getGlobalBounds().height / 2.f);
			text.setPosition(textPos);
			m_itemInfo.push_back(text);

			// Add new item to level
			level.addItem(std::move(std::unique_ptr<Item>(m_currentItem->clone())));

			return true;
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			sf::Vector2f mousePos = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));

			// Loop through existing sprites and see if the cursor collides with them
			for (int i = level.getItems().size() - 1; i >= 0; i--)
			{
				if (level.getItems()[i]->getSprite().getGlobalBounds().contains(mousePos))
				{
					level.removeItem(i);
					m_itemInfo.erase(m_itemInfo.begin() + i);
					return true;
				}
			}
		}
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::Z)
		{
			// Decrease sync ID
			--m_currentSyncID;
			m_currentSyncID = Utility::clampValue<int>(m_currentSyncID, -1, INT_MAX);

			m_currentSyncIDText.setString("Sync ID: " + std::to_string(m_currentSyncID));
		}
		else if (event.key.code == sf::Keyboard::X)
		{
			// Increase sync ID
			++m_currentSyncID;
			m_currentSyncID = Utility::clampValue<int>(m_currentSyncID, -1, INT_MAX);

			m_currentSyncIDText.setString("Sync ID: " + std::to_string(m_currentSyncID));
		}
	}

	return false;
}
bool EditorItemMode::update(float deltaTime, const sf::RenderWindow &editorWindow)
{
	sf::Vector2f mousePos = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));

	m_currentItem->setPosition(mousePos);

	return false;
}
void EditorItemMode::draw()
{
	m_currentItem->draw();
	Renderer::instance().drawHUD(m_currentIDText);
	Renderer::instance().drawHUD(m_currentIndexText);
	Renderer::instance().drawHUD(m_currentSyncIDText);

	for (std::size_t i = 0; i < m_itemInfo.size(); i++)
		Renderer::instance().drawForeground(m_itemInfo[i]);
}