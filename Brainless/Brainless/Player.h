#ifndef _PLAYER
#define _PLAYER

#include "Unit.h"

//version 0.2
class Player : public Unit
{
public:
	Player(sf::Vector2f startPosition);
	/* kollar efter playerinput, ska köras innan Unit::updateMovement */
	void checkPlayerInput();

	void updateAnimation(float deltaTime);

	enum AnimationState
	{
		noAnimation,
		idle,
		walk,
		run,
		slide,
		jump,
		fall
	};

	enum Direction
	{
		left,
		right
	};

	enum JumpState
	{
		null,
		pressed,
		released
	};


protected:

private:
	AnimationState m_state;
	Direction m_spriteDirection;
	Direction m_inputDirection;
	JumpState m_jumpState;
	/* gives an acceleration between minAcceleration and maxAcceleration depending on where value is between minValue and maxValue */
	static float calcAcceleration(float minAcceleration, float maxAcceleration, float useMinValue, float useMaxValue, float value);
};



#endif
