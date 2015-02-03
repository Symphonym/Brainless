#ifndef _PLAYER
#define _PLAYER

#include "Unit.h"

//version 0.2
class Player : public Unit
{
public:
	Player(sf::Vector2f startPosition);
	/* kollar efter playerinput, ska köras innan Unit::updateMovement */
	void checkPlayerInput(float deltaTime);

	void updateAnimation(float deltaTime);

	enum AnimationState
	{
		noAnimation,
		idle,
		startWalk,
		endWalk,
		run,
		turn,
		startJump,
		endJump,
		inAirUp,
		inAirFall,
		land
	};


	enum JumpState
	{
		ready,
		buttonPressed,
		inAir,
		landing
	};


protected:

private:
	AnimationState m_state;

	Direction m_inputDirection;
	JumpState m_jumpState;
	int m_jumpFrame;
	float m_jumpPower;
	/* gives an acceleration between minAcceleration and maxAcceleration depending on where value is between minValue and maxValue */
	static float calcAcceleration(float minAcceleration, float maxAcceleration, float useMinValue, float useMaxValue, float value);
	void jump();
};



#endif
