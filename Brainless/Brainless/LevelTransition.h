#ifndef INCLUDED_LEVEL_TRANSITION_H
#define INCLUDED_LEVEL_TRANSITION_H

#include <SFML\Graphics.hpp>

class Game;
class LevelTransition
{
public:

	LevelTransition(Game &game);

	void startTransition(int targetLevel, float fadeOutTime = 1.f);

	void update(float deltaTime);
	void draw();

	// Refresh screen covering textures, this only needs to be run
	// at startup or when the screen size changes
	void refreshTexture();

private:

	Game &m_game;

	sf::Texture m_coveringTexture;
	sf::Sprite m_coveringSprite;

	// If the level transition is ongoing
	bool m_isActive;

	// If we're fading out, otherwise we're fading in
	bool m_fadingOut;

	float m_curDelay;
	float m_maxDelay;

	// The level we're switching to
	int m_targetLevel;
};

#endif