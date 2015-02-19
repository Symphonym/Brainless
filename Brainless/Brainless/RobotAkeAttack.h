#ifndef INCLUDED_ROBOT_AKE_ATTACK_H
#define INCLUDED_ROBOT_AKE_ATTACK_H

#include "ArcadeGame.h"
#include "ArcadeMachine.h"
#include <SFML\Graphics.hpp>

class RobotAkeAttack : public ArcadeGame
{
public:

	RobotAkeAttack(ArcadeMachine &machine);

	virtual void onGameStart();

	virtual void events(const sf::Event &event);
	virtual void update(float deltaTime);
	virtual void draw();

private:
	int m_score;
};

#endif