#ifndef INCLUDED_BOOP_H
#define INCLUDED_BOOP_H

#include <SFML\Graphics.hpp>

#include "ArcadeGame.h"

class Boop : public ArcadeGame
{
public:

	Boop(ArcadeMachine &machine);

	virtual void onGameStart();

	virtual void update(float deltaTime);
	virtual void draw();

private:
	sf::Sprite m_background;
	sf::Sprite m_cloud1;
	sf::Sprite m_cloud2;
	sf::Sprite m_hand1;
	sf::Sprite m_hand2;

	sf::Vector2f m_screenPos;
	sf::Vector2f m_originalHandPos;
	float m_boopPos = 385;

	int m_score;
	sf::Text m_scoreText;

	float m_speed = 32;

	bool m_isBooped = false;
	bool m_isSoundPlaying = false;
};

#endif