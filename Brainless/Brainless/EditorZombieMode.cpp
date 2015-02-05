#include "EditorZombieMode.h"
#include "Utility.h"
#include "ResourceLoader.h"
#include "Renderer.h"

EditorZombieMode::EditorZombieMode()
{
	initializeSprites();
	m_highlightSprite.sprite.setTexture(ResourceLoader::instance().retrieveTexture("Zombie"));
	m_highlightSprite.type = 0;

	m_infoText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_infoText.setPosition(30, 100);
	m_infoText.setString("Placing zombies");
}

bool EditorZombieMode::events(const sf::Event &event, const sf::RenderWindow &editorWindow, Level &level)
{
	if (event.type == sf::Event::MouseWheelMoved)
	{
		// Scroll Zombie types
		m_highlightSprite.type += event.mouseWheel.delta;
		m_highlightSprite.type = Utility::clampValue<int>(m_highlightSprite.type, 0, Zombie_types - 1);
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		// Place a Zombie
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			EditorZombie editorZombie;
			editorZombie.sprite = m_highlightSprite.sprite;
			editorZombie.type = m_highlightSprite.type;

			level.addZombie(editorZombie);
			return true;
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			sf::Vector2f mousePos = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));

			// Loop through existing Zombies and see if the cursor collides with them
			for (int i = level.getZombies().size() - 1; i >= 0; i--)
			{
				if (level.getZombies()[i].sprite.getGlobalBounds().contains(mousePos))
				{
					level.removeZombie(i);
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
	Renderer::instance().drawForeground(m_highlightSprite.sprite);
	Renderer::instance().drawHUD(m_infoText);
}

void EditorZombieMode::addTexture(const std::string &textureName)
{
	m_availableDecorations.push_back(textureName);
}

void EditorZombieMode::initializeSprites()
{
	addTexture("GroundCube");
}