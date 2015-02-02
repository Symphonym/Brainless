#include "PopUpMenu.h"
#include "Level.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Constants.h"
#include <iostream>

PopUpMenu::PopUpMenu()
:
m_interactItem(nullptr),
m_isShowing(false),
m_position(sf::Vector2f(0,0))
{
	m_buttonsEnabled.reset();
	m_buttonsEnabled.set(ExamineIndex, true); // Examine is always enabled

	m_buttons[UseIndex].setTexture(ResourceLoader::instance().retrieveTexture("UseButton_Normal"));
	m_buttons[PickupIndex].setTexture(ResourceLoader::instance().retrieveTexture("PickupButton_Normal"));
	m_buttons[ExamineIndex].setTexture(ResourceLoader::instance().retrieveTexture("ExamineButton_Normal"));
}

void PopUpMenu::setPosition(const sf::Vector2f &pos)
{
	m_position = pos;

	// Update position of the popup buttons as well
	for (std::size_t i = 0; i < m_buttons.size(); i++)
		m_buttons[i].setPosition(m_position.x, m_position.y + m_buttons[i].getGlobalBounds().height*i);
}
void PopUpMenu::setItemCallback(PopUpMenu::ItemCallback callback)
{
	m_itemInteractCallback = callback;
}

void PopUpMenu::events(const sf::Event &event, const sf::RenderWindow &window, Level &level)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	// Key presses will hide the menu
	if (event.type == sf::Event::KeyReleased)
		m_isShowing = false;

	if (event.type == sf::Event::MouseButtonReleased)
	{

		// Show menu if right clicking on item/unit
		if (event.mouseButton.button == sf::Mouse::Right && !m_isShowing)
		{
			for (std::size_t i = 0; i < level.getItems().size(); i++)
			{
				sf::FloatRect itemBounds = level.getItems()[i]->getSprite().getGlobalBounds();
			
				// Right clicked on item
				if (itemBounds.contains(mousePos))
				{
					setPosition(mousePos);
					m_interactItem = level.getItems()[i].get();
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
				if (buttonBounds.contains(mousePos) && m_buttonsEnabled[i])
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
	}
}
void PopUpMenu::update(const sf::RenderWindow &window, const sf::Vector2f &playerOrigo)
{
	if (m_isShowing)
	{
		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		// Update texture of button
		for (std::size_t i = 0; i < m_buttons.size(); i++)
		{
			sf::FloatRect buttonBounds = m_buttons[i].getGlobalBounds();
			float distanceToPlayer = 0;

			// Calculate distance from player to item
			if (m_interactItem)
			{
				sf::Vector2f itemCenter(
					m_interactItem->getPosition().x + m_interactItem->getSprite().getGlobalBounds().width / 2.f,
					m_interactItem->getPosition().y + m_interactItem->getSprite().getGlobalBounds().height / 2.f);
				sf::Vector2f distanceVec = itemCenter - playerOrigo;

				distanceToPlayer = std::sqrt(distanceVec.x*distanceVec.x) + std::sqrt(distanceVec.y*distanceVec.y);
			}

			std::string textureName = "";
			if (i == UseIndex)
			{
				textureName = "UseButton";
				m_buttonsEnabled.set(i, distanceToPlayer <= Constants::InteractDistance);

			}
			else if (i == PickupIndex)
			{
				textureName = "PickupButton";
				m_buttonsEnabled.set(i, distanceToPlayer <= Constants::InteractDistance);
			}
			else if (i == ExamineIndex)
				textureName = "ExamineButton";

			// Change texture of button if it's enabled, otherwise set denied texture
			if (m_buttonsEnabled[i])
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && buttonBounds.contains(mousePos))
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
						m_buttons[i].setTexture(ResourceLoader::instance().retrieveTexture(textureName + "_Pressed"));
				}
				else
					m_buttons[i].setTexture(ResourceLoader::instance().retrieveTexture(textureName + "_Normal"));
			}
			else
				m_buttons[i].setTexture(ResourceLoader::instance().retrieveTexture(textureName + "_Denied"));
			
		}
	}

	/*if (!m_hitbox.contains(sf::Vector2i(mousePos)))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			m_isActive = false;
		}
	}*/
}

void PopUpMenu::draw()
{
	if (m_isShowing)
	{
		for (int i = 0; i < m_buttons.size(); i++)
			Renderer::instance().drawAbove(m_buttons[i]);
	}
}