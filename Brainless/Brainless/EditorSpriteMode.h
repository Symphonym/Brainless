#ifndef INCLUDED_EDITOR_SPRITE_MODE
#define INCLUDED_EDITOR_SPRITE_MODE

#include <SFML\Graphics.hpp>
#include "LevelSprite.h"
#include "Level.h"
#include <vector>

class EditorSpriteMode
{
public:

	EditorSpriteMode();

	// Returns true if something changed (so editor knows if it needs to resave)
	bool events(const sf::Event &event, const sf::RenderWindow &editorWindow, Level &level);
	bool update(float deltaTime, const sf::RenderWindow &editorWindow);
	void draw();

private:

	// Add a texture to the list of available decorations
	void addTexture(const std::string &textureName);

	// Initialize list of available sprites
	void initializeSprites();

	sf::Text m_layerText; // Shows what layer you're drawing to

	// Used as a preview to how it would look like if the sprite was placed
	LevelSprite m_highlightSprite;

	int m_currentSpriteIndex;
	std::vector<std::string> m_availableDecorations; // List of all decorations available (list of texture names)
};

#endif