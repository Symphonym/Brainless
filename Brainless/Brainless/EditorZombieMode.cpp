#include "EditorZombieMode.h"
#include "Utility.h"
#include "Constants.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "Unit.h"
#include "Zombie.h"
#include "IdleZombie.h"
#include "WalkingZombie.h"
#include "ChasingZombie.h"

EditorZombieMode::EditorZombieMode()
{
	m_highlightSprite.sprite.setTexture(ResourceLoader::instance().retrieveTexture("Zombie"));
	m_highlightSprite.type = 0;

	m_infoText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_infoText.setPosition(30, 100);
	m_infoText.setString("Placing zombies");
}

bool EditorZombieMode::events(const sf::Event &event, const sf::RenderWindow &editorWindow, Level &level)
{
	//Adding mask that indicate speicall abilities of zombies
	while (m_zombieMasks.size() != 0)
	{
		m_zombieMasks.pop_back();
	}
	for (int i = 0; i < level.getUnits().size(); i++)
	{
		EditorZombie temp1,temp2;
		switch (level.getUnit(i).getUnitType())
		{
		case Unit::ID_WalkingZombie:
			temp1.sprite = level.getUnit(i).getSprite();
			temp1.sprite.setPosition(temp1.sprite.getPosition() + sf::Vector2f((dynamic_cast<WalkingZombie*>(&level.getUnit(i)))->getWalkLenght(), 0));
			temp1.sprite.setColor(sf::Color(255, 255, 255, 128));
			m_zombieMasks.push_back(temp1);
			break;
		case Unit::ID_ChasingZombie:
			temp1.sprite = level.getUnit(i).getSprite(); temp2.sprite = level.getUnit(i).getSprite();
			temp1.sprite.setPosition(temp1.sprite.getPosition() + sf::Vector2f((dynamic_cast<ChasingZombie*>(&level.getUnit(i)))->getWalkLenght(), 0));
			temp2.sprite.setPosition(temp2.sprite.getPosition() - sf::Vector2f((dynamic_cast<ChasingZombie*>(&level.getUnit(i)))->getWalkLenght(), 0));
			temp1.sprite.setColor(sf::Color(255, 255, 255, 128));
			temp2.sprite.setColor(sf::Color(255, 255, 255, 128));
			m_zombieMasks.push_back(temp1);
			m_zombieMasks.push_back(temp2);
			break;
		}
	}
	if (event.type == sf::Event::MouseWheelMoved)
	{
		// Scroll Zombie types
		m_highlightSprite.type += event.mouseWheel.delta;
		m_highlightSprite.type = Utility::clampValue<int>(m_highlightSprite.type, 0, Constants::ZombieTypeCount-1);
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		// Place a Zombie
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			//Create zombie mask untill all variables are decided
			if (!zombie_created)
			{
				zombie_created = true;
				m_createdZombie.sprite = m_highlightSprite.sprite;
				m_createdZombie.type = m_highlightSprite.type;
				m_createdZombie.sprite.setTextureRect(m_highlightSprite.sprite.getTextureRect());
				m_createdZombie.sprite.setColor(sf::Color(255, 255, 255, 128));
				return false;
			}
			//Create the zombie & add it to level
			else
			{
				zombie_created = false;
				sf::Vector2f mousePos = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));
				Unit* temp;
				Unit::Direction temp_direction = Unit::dir_right;
				switch (m_createdZombie.type + 1)
				{
				case Unit::ID_IdleZombie: //Idle zombie
					if (mousePos.x - m_createdZombie.sprite.getPosition().x < 0)
						temp_direction = Unit::dir_left;
					temp = new IdleZombie(m_createdZombie.sprite.getPosition() + sf::Vector2f(85, 50), temp_direction);
					temp->addTexture(ResourceLoader::instance().retrieveTexture("Zombie"));
					temp->updateAnimation(0);
					level.addUnit(std::move(Level::UnitPtr(temp)));
					break;
				case Unit::ID_WalkingZombie: //Walking zombie
					m_createdZombie.walk_distance = mousePos.x - m_createdZombie.sprite.getPosition().x;
					temp = new WalkingZombie(m_createdZombie.sprite.getPosition() + sf::Vector2f(85, 50), m_createdZombie.walk_distance);
					temp->addTexture(ResourceLoader::instance().retrieveTexture("Zombie"));
					temp->updateAnimation(0);
					level.addUnit(std::move(Level::UnitPtr(temp)));
					break;
				case Unit::ID_ChasingZombie: //Chasing Zombie
					m_createdZombie.walk_distance = mousePos.x - m_createdZombie.sprite.getPosition().x;
					temp = new ChasingZombie(m_createdZombie.sprite.getPosition() + sf::Vector2f(85, 50), m_createdZombie.walk_distance);
					temp->addTexture(ResourceLoader::instance().retrieveTexture("Zombie"));
					temp->updateAnimation(0);
					level.addUnit(std::move(Level::UnitPtr(temp)));
					break;
				}
				return true;
			}
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			sf::Vector2f mousePos = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));

			// Loop through existing Zombies and see if the cursor collides with them
			for (int i = level.getUnits().size() - 1; i >= 0; i--)
			{
				if (level.getUnits()[i]->getSprite().getGlobalBounds().contains(mousePos))
				{
					level.removeUnit(i);
					return true;
				}
			}
		}
	}

	return false;
}

bool EditorZombieMode::update(float deltaTime, const sf::RenderWindow &editorWindow)
{
	sf::Vector2f mousePos = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));
	m_highlightSprite.sprite.setPosition(mousePos);
	switch ((Unit::UnitType)(m_highlightSprite.type + 1))
	{
	case Unit::ID_IdleZombie:
		m_infoText.setString("Idle Zombie");
		m_highlightSprite.sprite.setTextureRect(sf::IntRect(0, 256, 256, 256));
		break;
	case Unit::ID_WalkingZombie:
		m_infoText.setString("Walking Zombie");
		m_highlightSprite.sprite.setTextureRect(sf::IntRect(0, 0, 256, 256));
		break;
	case Unit::ID_ChasingZombie:
		m_infoText.setString("Chasing Zombie");
		m_highlightSprite.sprite.setTextureRect(sf::IntRect(0, 512, 256, 256));
		break;
	default:
		m_infoText.setString("Unidentified selection");
		break;
	}

	return false;
}
void EditorZombieMode::draw()
{

	Renderer::instance().drawDepth(m_highlightSprite.sprite);
	if (zombie_created)
	{
		Renderer::instance().drawForeground(m_createdZombie.sprite);
	}
	for (int i = 0; i < m_zombieMasks.size(); i++)
	{
		Renderer::instance().drawForeground(m_zombieMasks[i].sprite);
	}
	Renderer::instance().drawHUD(m_infoText);
}