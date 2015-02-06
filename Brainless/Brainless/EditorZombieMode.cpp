#include "EditorZombieMode.h"
#include "Utility.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "Unit.h"
#include "Zombie.h"
#include "IdleZombie.h"
#include "WalkingZombie.h"

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
	//Replace mask
	while (m_zombieMasks.size()!=0)
	{
		//delete m_zombieMasks[m_zombieMasks.size() - 1];
		m_zombieMasks.pop_back();
	}
	for (int i = 0; i < level.getUnits().size(); i++)
	{
		EditorZombie temp;
		temp.sprite = level.getUnit(i).getSprite();
		if (level.getUnit(i).getUnitType() == Unit::ID_WalkingZombie)
			temp.sprite.setPosition(temp.sprite.getPosition() + sf::Vector2f((dynamic_cast<WalkingZombie*>(&level.getUnit(i)))->getWalkLenght(), 0));
		temp.sprite.setColor(sf::Color(255, 255, 255, 128));
		m_zombieMasks.push_back(temp);
	}
	if (event.type == sf::Event::MouseWheelMoved)
	{
		// Scroll Zombie types
		m_highlightSprite.type = event.mouseWheel.delta;
		m_highlightSprite.type = Utility::clampValue<int>(m_highlightSprite.type, 0, Zombie_types - 1);
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		// Place a Zombie
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			if (!zombie_created)
			{
				zombie_created = true;
				m_createdZombie.sprite = m_highlightSprite.sprite;
				m_createdZombie.type = m_highlightSprite.type;
				m_createdZombie.sprite.setTextureRect(m_highlightSprite.sprite.getTextureRect());
				m_createdZombie.sprite.setColor(sf::Color(255, 255, 255, 128));
				return false;
			}
			else
			{
				zombie_created = false;
				sf::Vector2f mousePos = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));
				Unit* temp;
				switch (m_createdZombie.type)
				{
				case 0: //Walking zombie
					m_createdZombie.walk_distance = mousePos.x - m_createdZombie.sprite.getPosition().x;
					temp = new WalkingZombie(m_createdZombie.sprite.getPosition() + sf::Vector2f(85,50), m_createdZombie.walk_distance);
					temp->addTexture(ResourceLoader::instance().retrieveTexture("Zombie"));
					temp->updateAnimation(0);
					level.addUnit(std::move(Level::UnitPtr(temp)));
					break;
				case 1: //Idle zombie
					temp = new IdleZombie(m_createdZombie.sprite.getPosition() + sf::Vector2f(85, 50));
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
	m_highlightSprite.sprite.setTextureRect(sf::IntRect(0, 256 * m_highlightSprite.type, 256, 256));

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