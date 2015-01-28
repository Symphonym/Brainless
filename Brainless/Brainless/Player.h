#ifndef _PLAYER
#define _PLAYER

#include "Unit.h"

//version 0.1
class Player : public Unit
{
public:
	Player(float startX, float startY, float maxSpeedX, float maxSpeedY);
	Player(float startX, float startY, int width, int height, float maxSpeedX, float maxSpeedY);

	/* kollar efter playerinput, ska köras innan Unit::updateMovement */
	void checkPlayerInput();

	void updateAnimation(float deltaTime);

	enum AnimationState
	{
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
};



#endif
