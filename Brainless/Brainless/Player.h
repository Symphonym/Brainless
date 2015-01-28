#ifndef _PLAYER
#define _PLAYER

#include "Unit.h"
#include "Animation.h"

#include <SFML\Graphics.hpp>

class Player : public Unit
{
public:
	Player(float startX, float startY, float maxSpeedX, float maxSpeedY);
	Player(float startX, float startY, int width, int height, float maxSpeedX, float maxSpeedY);

	/* kollar efter playerinput */
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


protected:
	Animation m_animation;
private:
	AnimationState m_state;
	Direction m_spriteDirection;
	Direction m_inputDirection;
};



#endif
