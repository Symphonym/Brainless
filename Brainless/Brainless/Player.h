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
	/* kollar efter keyboardinput, och eventuel slide*/
	void checkPlayerInput();
	void updateAnimation(float deltaTime);

	enum AnimationState
	{
		idle,
		walk,
		run,
		slide, //(turning while running)
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
	Direction m_direction;
	/* gives a speed between minSpeed and maxSpeed depending on where value is between minValue and maxValue */
	float calcSpeed(float minSpeed, float maxSpeed, float minValue, float maxValue, float value);
};



#endif