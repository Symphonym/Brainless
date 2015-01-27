#ifndef INCLUDED_EDITOR_SPRITE_MODE
#define INCLUDED_EDITOR_SPRITE_MODE

#include <SFML\Graphics.hpp>
#include <vector>

class EditorSpriteMode
{
public:

	EditorSpriteMode(std::vector<std::pair<sf::Sprite, bool> >& spriteVector);

	void events(const sf::Event &event);

	// Returns true if sprites were removed/added (something changed)
	bool update(float deltaTime, const sf::RenderWindow &editorWindow);
	void draw();

private:

	// Add a texture to the list of available decorations
	void addTexture(const std::string &textureName);

	// Initialize list of available sprites
	void initializeSprites();

	bool m_somethingChanged;


	// Used as a preview to how it would look like if the sprite was placed
	sf::Sprite m_highlightSprite;

	// Whether or not to draw to the foreground, otherwise draw to background
	bool m_drawToForeground;

	int m_currentSpriteIndex;
	std::vector<std::string> m_availableDecorations; // List of all decorations available (list of texture names)

	// The list of sprites for the level
	std::vector<std::pair<sf::Sprite, bool> >& m_sprites;
};

#endif