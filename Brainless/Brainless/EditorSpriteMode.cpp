#include "EditorSpriteMode.h"
#include "Utility.h"
#include "ResourceLoader.h"
#include "Renderer.h"

EditorSpriteMode::EditorSpriteMode()
{
	initializeSprites();

	m_highlightSprite.sprite.setTexture(ResourceLoader::instance().retrieveTexture(m_availableDecorations[m_currentSpriteIndex]));
	m_highlightSprite.textureName = m_availableDecorations[m_currentSpriteIndex];

	m_layerText.setFont(ResourceLoader::instance().retrieveFont("EditorFont"));
	m_layerText.setPosition(30, 100);
	m_layerText.setString("Drawing to: FOREGROUND");
	m_highlightSprite.drawToForeground = true;
}

bool EditorSpriteMode::events(const sf::Event &event, const sf::RenderWindow &editorWindow, Level &level)
{
	if (event.type == sf::Event::MouseWheelMoved)
	{
		// Scroll between block types
		m_currentSpriteIndex += event.mouseWheel.delta;
		m_currentSpriteIndex = Utility::clampValue<int>(m_currentSpriteIndex, 0, m_availableDecorations.size() - 1);

		
		m_highlightSprite.sprite.setTexture(ResourceLoader::instance().retrieveTexture(m_availableDecorations[m_currentSpriteIndex]));
		m_highlightSprite.textureName = m_availableDecorations[m_currentSpriteIndex];
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		// Place a sprite
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			LevelSprite levelSprite;
			levelSprite.sprite = m_highlightSprite.sprite;
			levelSprite.drawToForeground = m_highlightSprite.drawToForeground;
			levelSprite.textureName = m_highlightSprite.textureName;

			level.addDecoration(levelSprite);
			return true;
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			sf::Vector2f mousePos = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));

			// Loop through existing sprites and see if the cursor collides with them
			for (int i = level.getDecorations().size()-1; i >= 0; i--)
			{
				if (level.getDecorations()[i].sprite.getGlobalBounds().contains(mousePos))
				{
					level.removeDecoration(i);
					return true;
				}
			}
		}
	}
	
	return false;
}

bool EditorSpriteMode::update(float deltaTime, const sf::RenderWindow &editorWindow)
{
	sf::Vector2f mousePos = editorWindow.mapPixelToCoords(sf::Mouse::getPosition(editorWindow));

	m_highlightSprite.sprite.setPosition(mousePos);


	// Change between foreground/background
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
	{
		m_highlightSprite.drawToForeground = true;
		m_layerText.setString("Drawing to: FOREGROUND");
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
	{
		m_highlightSprite.drawToForeground = false;
		m_layerText.setString("Drawing to: BACKGROUND");
	}

	return false;
}
void EditorSpriteMode::draw()
{
	if (m_highlightSprite.drawToForeground)
		Renderer::instance().drawForeground(m_highlightSprite.sprite);
	else
		Renderer::instance().drawBackground(m_highlightSprite.sprite);

	Renderer::instance().drawHUD(m_layerText);
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