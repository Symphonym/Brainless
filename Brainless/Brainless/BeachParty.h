#ifndef INCLUDED_BEACH_PARTY_H
#define INCLUDED_BEACH_PARTY_H

#include <SFML\Graphics.hpp>

#include "ArcadeGame.h"
#include "Animation.h"

class BeachParty : public ArcadeGame
{
public:

	BeachParty(ArcadeMachine &machine);

	virtual void events(const sf::Event &event) {};
	virtual void update(float deltaTime);
	virtual void draw();

private:
	sf::Sprite m_turtleSprite;
	Animation m_turtleAnimation;

	sf::Sprite m_frieSprite;
	Animation m_frieAnimation;

	sf::Texture m_crabTexture;
	Animation m_crabAnimation;

	sf::Sprite m_background;

	sf::Vector2f m_newPos;
	sf::Vector2f m_screenPos;

	std::vector<sf::Sprite> m_crabs;
	std::vector<bool> m_crabDirections;

	int m_score;
	sf::Text m_scoreText;

	float m_speed = 100;

	bool m_isDead = false;
};

#endif