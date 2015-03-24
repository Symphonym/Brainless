#include "PopUpMenu.h"
#include "Level.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Constants.h"
#include "Cursor.h"
#include "Game.h"
#include "Notification.h"

PopUpMenu::PopUpMenu()
:
m_interactItem(nullptr),
m_isShowing(false),
m_position(sf::Vector2f(0,0))
{
	m_buttons[UseIndex].setTexture(ResourceLoader::instance().retrieveTexture("UseButton_Normal"));
	m_buttons[PickupIndex].setTexture(ResourceLoader::instance().retrieveTexture("PickupButton_Normal"));
	m_buttons[ExamineIndex].setTexture(ResourceLoader::instance().retrieveTexture("ExamineButton_Normal"));
	m_background.setTexture(ResourceLoader::instance().retrieveTexture("background_Notepad"));
}

void PopUpMenu::setPosition(const sf::Vector2f &pos)
{
	m_position = pos;
	// Update position of the popup buttons as well
	m_background.setPosition(pos + sf::Vector2f(0,-60));
	for (std::size_t i = 0; i < m_buttons.size(); i++)
		m_buttons[i].setPosition(m_position.x, m_position.y + (m_buttons[i].getGlobalBounds().height+10)*i);
}
void PopUpMenu::setItemCallback(PopUpMenu::ItemCallback callback)
{
	m_itemInteractCallback = callback;
}

void PopUpMenu::events(const sf::Event &event, Game &game)
{
	sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(sf::Mouse::getPosition(game.getWindow()));

	// Key presses will hide the menu
	if (event.type == sf::Event::KeyReleased)
		m_isShowing = false;

	if (event.type == sf::Event::MouseButtonReleased)
	{
		// Show menu if left clicking on item/unit, and the player isn't holding an item (to interact with stuff)
		if (event.mouseButton.button == sf::Mouse::Left && !m_isShowing && !game.holdingItem())
		{

			// Do backwards to take the topmost item first
			for (int i = game.getLevel().getItems().size()-1; i >= 0; i--)
			{
				sf::FloatRect itemBounds = game.getLevel().getItems()[i]->getInteractBounds();
			
				// left clicked on item
				if (itemBounds.contains(mousePos))
				{
					setPosition(mousePos);
					m_interactItem = game.getLevel().getItems()[i].get();
					m_isShowing = true;
					break;
				}
			}
		}
		else if (event.mouseButton.button == sf::Mouse::Left && m_isShowing)
		{
			for (std::size_t i = 0; i < m_buttons.size(); i++)
			{
				sf::FloatRect buttonBounds = m_buttons[i].getGlobalBounds();
				if (buttonBounds.contains(mousePos)/* && m_buttonsEnabled[i]*/)
				{
					InteractTypes interactType = InteractTypes::Nothing;
					if (i == UseIndex)
						interactType = InteractTypes::Use;
					else if (i == PickupIndex)
						interactType = InteractTypes::Pickup;
					else if (i == ExamineIndex)
						interactType = InteractTypes::Examine;

					// Call the callback function
					if (m_itemInteractCallback)
						m_itemInteractCallback(m_interactItem, interactType);
					m_interactItem = nullptr;
					break;
				}
			}

			// Left clicking will hide the menu no matter where you click
			m_isShowing = false;
		}

		// Right click examines an item
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			// Do backwards to take the topmost item first
			for (int i = game.getLevel().getItems().size() - 1; i >= 0; i--)
			{
				sf::FloatRect itemBounds = game.getLevel().getItems()[i]->getInteractBounds();

				// Right clicked on an item
				if (itemBounds.contains(mousePos))
				{
					game.getLevel().getItems()[i]->onExamine();
					Notification::instance().write(game.getLevel().getItems()[i]->getExamineString());
					break;
				}
			}
		}
	}
}
void PopUpMenu::update(Game &game, const sf::Vector2f &playerOrigo)
{
	sf::Vector2f mousePos = game.getWindow().mapPixelToCoords(sf::Mouse::getPosition(game.getWindow()));

	if (m_isShowing)
	{

		// Update texture of button
		for (std::size_t i = 0; i < m_buttons.size(); i++)
		{
			sf::FloatRect buttonBounds = m_buttons[i].getGlobalBounds();

			std::string textureName = "";
			if (i == UseIndex)
				textureName = "UseButton";
			else if (i == PickupIndex)
				textureName = "PickupButton";
			else if (i == ExamineIndex)
				textureName = "ExamineButton";

			// Change texture of button
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && buttonBounds.contains(mousePos))
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					m_buttons[i].setTexture(ResourceLoader::instance().retrieveTexture(textureName + "_Pressed"));
			}
			else
				m_buttons[i].setTexture(ResourceLoader::instance().retrieveTexture(textureName + "_Normal"));
			
		}
	}
}

void PopUpMenu::draw()
{
	if (m_isShowing)
	{
		Renderer::instance().drawPopUp(m_background);
	//	Renderer::instance().drawHUD(m_background);
		for (std::size_t i = 0; i < m_buttons.size(); i++)
			Renderer::instance().drawPopUp(m_buttons[i]);
	//		Renderer::instance().drawHUD(m_buttons[i]);
	}
}