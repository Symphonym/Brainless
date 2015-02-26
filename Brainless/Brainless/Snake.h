#ifndef INCLUDED_SNAKE_H
#define INCLUDED_SNAKE_H

#include <SFML\Graphics.hpp>

#include "ArcadeGame.h"
#include "Animation.h"

class Snake : public ArcadeGame
{
public:

	Snake(ArcadeMachine &machine);

	virtual void onGameStart();

	virtual void update(float deltaTime);
	virtual void draw();

private:
	sf::Sprite m_turtleSprite;
	Animation m_turtleAnimation;

	sf::Sprite m_frieSprite;
	Animation m_frieAnimation;

	sf::Sprite m_background;

	sf::Vector2f m_screenPos;

	int m_score;
	sf::Text m_scoreText;
	sf::Text m_infoText;
	bool m_infoShowing = true;

	bool m_isDead = false;
};

#endif