#include "EditorItemMode.h"
#include "Item.h"
#include "ResourceLoader.h"
#include "ItemDatabase.h"
#include "Utility.h"
#include "Renderer.h"

EditorItemMode::EditorItemMode()
:
m_currentIndex(1),
m_currentSyncID(-1)
{
	m_currentNameText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_currentIndexText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_currentSyncIDText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_currentIDText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));

	m_currentNameText.setCharacterSize(17);
	m_currentIndexText.setCharacterSize(17);
	m_currentSyncIDText.setCharacterSize(17);
	m_currentIDText.setCharacterSize(17);

	m_currentIndexText.setString("Item index: " + std::to_string(m_currentIndex));
	m_currentSyncIDText.setString("Sync ID: " + std::to_string(m_currentSyncID));

	// Initialize itemID text
	m_currentNameText.setPosition(10, 50);
	m_currentIDText.setPosition(10, 75);
	m_currentIndexText.setPosition(10, 100);
	m_currentSyncIDText.setPosition(10, 125);

	// Initialize current item and the starting text of currentID with that item
	m_currentItem = std::move(ItemDatabase::instance().extractItemByCount(m_currentIndex));
	m_currentIDText.setString("Item ID: " + std::to_string(m_currentItem->getID()));
	m_currentNameText.setString("Item name: " + m_currentItem->getName());


	sf::Image bgImg;
	bgImg.create(300, 170, sf::Color(0, 0, 0, 200));
	m_infoBackground.loadFromImage(bgImg);
	m_bgSprite.setTexture(m_infoBackground);
}


void EditorItemMode::reloadDebugText(Level &level)
{
	m_itemInfo.clear();
	m_itemInteractBounds.clear();
	m_itemCollisionBounds.clear();

	// Load debug strings for any items already loaded
	for (std::size_t i = 0; i < level.getItems().size(); i++)
	{
		Item& curItem = *level.getItems()[i].get();

		sf::Text text;
		text.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
		text.setString("Name: " + curItem.getName() + "\nID: " + std::to_string(curItem.getID()) + "\nSyncID: " + std::to_string(curItem.getSyncID()) + "\n" + curItem.getToggleString());
		text.setCharacterSize(14);

		sf::Vector2f textPos(
			curItem.getInteractBounds().left,
			curItem.getInteractBounds().top + curItem.getInteractBounds().height);
		text.setPosition(textPos);
		m_itemInfo.push_back(text);

		sf::RectangleShape interactShape = sf::RectangleShape(sf::Vector2f(curItem.getInteractBounds().width, curItem.getInteractBounds().height));
		interactShape.setFillColor(sf::Color::Color(0, 255, 0, 30));
		interactShape.setOutlineThickness(1);
		interactShape.setOutlineColor(sf::Color::Green);
		interactShape.setPosition(curItem.getInteractBounds().left, curItem.getInteractBounds().top);
		m_itemInteractBounds.push_back(interactShape);

		sf::RectangleShape collisionShape = sf::RectangleShape(sf::Vector2f(curItem.getCollisionBounds().width, curItem.getCollisionBounds().height));
		collisionShape.setFillColor(sf::Color::Color(255, 0, 0, 30));
		collisionShape.setOutlineThickness(1);
		collisionShape.setOutlineColor(sf::Color::Red);
		collisionShape.setPosition(curItem.getCollisionBounds().left, curItem.getCollisionBounds().top);
		m_itemCollisionBounds.push_back(collisionShape);
	}


}

bool EditorItemMode::events(const sf::Event &event, const sf::RenderWindow &editorWindow, Level &level)
{
	if (event.type == sf::Event::MouseWheelMoved)
	{
		sf::Vector2f mousePos = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));
		bool toggled = false;
		// Loop through all items and see if you scrolled on any of them
		for (int i = level.getItems().size() - 1; i >= 0; i--)
		{
			if (level.getItems()[i]->getSprite().getGlobalBounds().contains(mousePos))
			{
				if (level.getItems()[i]->onToggle(event.mouseWheel.delta))
				{
					toggled = true;
					m_itemInfo[i].setString("Name: " + level.getItems()[i]->getName() + "\nID: " + std::to_string(level.getItems()[i]->getID()) + "\nSyncID: " + std::to_string(level.getItems()[i]->getSyncID()) + "\n" + level.getItems()[i]->getToggleString());
					break;
				}
			}
		}
		if (!toggled)
		{
			// Scroll between items
			m_currentIndex += event.mouseWheel.delta;
			m_currentIndex = Utility::clampValue<int>(m_currentIndex, 1, ItemDatabase::instance().getItemCount());

			// Update preview item
			m_currentItem = std::move(ItemDatabase::instance().extractItemByCount(m_currentIndex));

			m_currentNameText.setString("Item name: " + m_currentItem->getName());
			m_currentIDText.setString("Item ID: " + std::to_string(m_currentItem->getID()));
			m_currentIndexText.setString("Item index: " + std::to_string(m_currentIndex));
		}
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
			text.setString("Name: " + newItem->getName() + "\nID: " + std::to_string(newItem->getID()) + "\nSyncID: " + std::to_string(newItem->getSyncID()) + "\n" + newItem->getToggleString());
			text.setCharacterSize(14);

			sf::Vector2f textPos(
				newItem->getInteractBounds().left,
				newItem->getInteractBounds().top + newItem->getInteractBounds().height);
			text.setPosition(textPos);
			m_itemInfo.push_back(text);

			sf::RectangleShape interactShape = sf::RectangleShape(sf::Vector2f(newItem->getInteractBounds().width, newItem->getInteractBounds().height));
			interactShape.setFillColor(sf::Color::Color(0, 255, 0, 30));
			interactShape.setOutlineThickness(1);
			interactShape.setOutlineColor(sf::Color::Green);
			interactShape.setPosition(newItem->getInteractBounds().left, newItem->getInteractBounds().top);
			m_itemInteractBounds.push_back(interactShape);

			sf::RectangleShape collisionShape = sf::RectangleShape(sf::Vector2f(newItem->getCollisionBounds().width, newItem->getCollisionBounds().height));
			collisionShape.setFillColor(sf::Color::Color(255, 0, 0, 30));
			collisionShape.setOutlineThickness(1);
			collisionShape.setOutlineColor(sf::Color::Red);
			collisionShape.setPosition(newItem->getCollisionBounds().left, newItem->getCollisionBounds().top);
			m_itemCollisionBounds.push_back(collisionShape);

			//sf::VertexArray boundVec = sf::VertexArray(sf::PrimitiveType::LinesStrip, 4);
			//boundVec[0] = sf::Vertex(sf::Vector2f(newItem->getInteractBounds().left, newItem->getInteractBounds().top), sf::Color::Red);
			//boundVec[1] = sf::Vertex(sf::Vector2f(newItem->getInteractBounds().left + newItem->getInteractBounds().width, newItem->getInteractBounds().top), sf::Color::Red);
			//boundVec[2] = sf::Vertex(sf::Vector2f(newItem->getInteractBounds().left + newItem->getInteractBounds().width, newItem->getInteractBounds().top + newItem->getInteractBounds().height), sf::Color::Red);
			//boundVec[3] = sf::Vertex(sf::Vector2f(newItem->getInteractBounds().left, newItem->getInteractBounds().top + newItem->getInteractBounds().height), sf::Color::Red);
			//m_itemBounds.push_back(boundVec);

			// Add new item to level
			level.addItem(std::move(newItem));

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
					m_itemInteractBounds.erase(m_itemInteractBounds.begin() + i);
					m_itemCollisionBounds.erase(m_itemCollisionBounds.begin() + i);
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
	Renderer::instance().drawHUD(m_bgSprite);
	Renderer::instance().drawHUD(m_currentNameText);
	Renderer::instance().drawHUD(m_currentIDText);
	Renderer::instance().drawHUD(m_currentIndexText);
	Renderer::instance().drawHUD(m_currentSyncIDText);

	for (std::size_t i = 0; i < m_itemInfo.size(); i++)
		Renderer::instance().drawForeground(m_itemInfo[i]);
	for (std::size_t i = 0; i < m_itemInteractBounds.size(); i++)
		Renderer::instance().drawAbove(m_itemInteractBounds[i]);
	for (std::size_t i = 0; i < m_itemCollisionBounds.size(); i++)
		Renderer::instance().drawAbove(m_itemCollisionBounds[i]);
}