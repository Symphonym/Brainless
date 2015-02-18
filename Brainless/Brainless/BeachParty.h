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
	virtual void update(float deltaTime) {};
	virtual void draw() {};

private:
	sf::Sprite m_turtleSprite;
	Animation m_turtleAnimation;

	sf::Sprite m_frieSprite;
	Animation m_frieAnimation;

	sf::Sprite m_crabSprite;
	Animation m_crabAnimation;
};

#endif