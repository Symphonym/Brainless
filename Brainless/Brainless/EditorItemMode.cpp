#include "EditorItemMode.h"
#include "Item.h"
#include "ResourceLoader.h"
#include "ItemDatabse.h"
#include "Utility.h"
#include "Renderer.h"

EditorItemMode::EditorItemMode(std::vector<std::unique_ptr<Item> >& itemVector)
:
m_items(itemVector),
m_currentIndex(1),
m_currentSyncID(-1)
{
	m_currentIndexText.setFont(ResourceLoader::instance().retrieveFont("EditorFont"));
	m_currentSyncIDText.setFont(ResourceLoader::instance().retrieveFont("EditorFont"));
	m_currentIDText.setFont(ResourceLoader::instance().retrieveFont("EditorFont"));

	m_currentIndexText.setString("Item index: " + std::to_string(m_currentIndex));
	m_currentSyncIDText.setString("Sync ID: " + std::to_string(m_currentSyncID));

	m_currentIDText.setPosition(10, 50);
	m_currentIndexText.setPosition(10, 100);
	m_currentSyncIDText.setPosition(10, 150);



	m_currentItem = std::move(ItemDatabase::instance().extractItemByCount(m_currentIndex));
	m_currentIDText.setString("Item ID: " + std::to_string(m_currentItem->getID()));

}
bool EditorItemMode::events(const sf::Event &event, const sf::RenderWindow &editorWindow)
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
			m_items.push_back(std::move(std::unique_ptr<Item>(m_currentItem->clone())));
			m_items.back()->setSyncID(m_currentSyncID);

			sf::Text text;
			text.setFont(ResourceLoader::instance().retrieveFont("EditorFont"));
			text.setString("ID: " + std::to_string(m_items.back()->getID()) + " SyncID: " + std::to_string(m_items.back()->getSyncID()));

			sf::Vector2f textPos(
				m_items.back()->getSprite().getPosition().x + m_items.back()->getSprite().getGlobalBounds().width / 2.f - text.getGlobalBounds().width/2.f,
				m_items.back()->getSprite().getPosition().y - m_items.back()->getSprite().getGlobalBounds().height/2.f);
			text.setPosition(textPos);
			m_itemInfo.push_back(text);
			return true;
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			sf::Vector2f mousePos = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));

			// Loop through existing sprites and see if the cursor collides with them
			for (int i = m_items.size() - 1; i >= 0; i--)
			{
				if (m_items[i]->getSprite().getGlobalBounds().contains(mousePos))
				{
					m_items.erase(m_items.begin() + i);
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