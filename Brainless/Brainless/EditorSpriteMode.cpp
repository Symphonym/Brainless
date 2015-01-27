#include "EditorSpriteMode.h"
#include "Utility.h"
#include "ResourceLoader.h"
#include "Renderer.h"

EditorSpriteMode::EditorSpriteMode(std::vector<std::pair<sf::Sprite,bool> >& spriteVector)
:
m_somethingChanged(false),
m_drawToForeground(true),
m_sprites(spriteVector)
{
	initializeSprites();

	m_highlightSprite.setTexture(ResourceLoader::instance().retrieveTexture(m_availableDecorations[m_currentSpriteIndex]));
}

bool EditorSpriteMode::events(const sf::Event &event, const sf::RenderWindow &editorWindow)
{
	if (event.type == sf::Event::MouseWheelMoved)
	{
		// Scroll between block types
		m_currentSpriteIndex += event.mouseWheel.delta;
		m_currentSpriteIndex = Utility::clampValue<int>(m_currentSpriteIndex, 0, m_availableDecorations.size() - 1);

		m_highlightSprite.setTexture(ResourceLoader::instance().retrieveTexture(m_availableDecorations[m_currentSpriteIndex]));
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		// Place a sprite
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			m_sprites.push_back(std::make_pair(m_highlightSprite, m_drawToForeground));
			return true;
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			sf::Vector2f mousePos = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));

			// Loop through existing sprites and see if the cursor collides with them
			for (std::size_t i = 0; i < m_sprites.size(); i++)
			{
				if (m_sprites[i].first.getGlobalBounds().contains(mousePos))
				{
					m_sprites.erase(m_sprites.begin() + i);
					return true;
				}
			}

			return false;
		}
	}
	else
		return false;
}

// Returns true if sprites were removed/added (something changed)
bool EditorSpriteMode::update(float deltaTime, const sf::RenderWindow &editorWindow)
{
	sf::Vector2f mousePos = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));

	m_highlightSprite.setPosition(mousePos);


	// Change between foreground/background
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		m_drawToForeground = true;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		m_drawToForeground = false;

	return false;
	// Add/remove a sprite with left/right click
	/*if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_sprites.push_back(std::make_pair(m_highlightSprite, m_drawToForeground));
		return true;
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		for (std::size_t i = 0; i < m_sprites.size(); i++)
		{
			if (m_sprites[i].first.getGlobalBounds().contains(mousePos))
			{
				m_sprites.erase(m_sprites.begin() + i);
				return true;
			}
		}
		return false;
	}
	else
		return false;*/
}
void EditorSpriteMode::draw()
{
	if (m_drawToForeground)
		Renderer::instance().drawForeground(m_highlightSprite);
	else
		Renderer::instance().drawBackground(m_highlightSprite);
}



void EditorSpriteMode::addTexture(const std::string &textureName)
{
	m_availableDecorations.push_back(textureName);
}

void EditorSpriteMode::initializeSprites()
{
	ResourceLoader::instance().loadTexture("GroundCube", "cube.png");
	ResourceLoader::instance().loadTexture("RedCube", "cubeRed.png");
	ResourceLoader::instance().loadTexture("BlueCube", "cubeBlue.png");

	addTexture("GroundCube");
	addTexture("RedCube");
	addTexture("BlueCube");
}