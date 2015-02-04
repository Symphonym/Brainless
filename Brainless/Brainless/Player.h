#ifndef _PLAYER
#define _PLAYER

#include "Unit.h"

//version 0.2
class Player : public Unit
{
public:
	Player(sf::Vector2f startPosition);
	/* kollar efter playerinput, ska köras innan Unit::updateMovement */
	void updateTask(float deltaTime);

	void updateAnimation(float deltaTime);

	void takesDamage(sf::Vector2f collisionPos);

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
		land,
		dying,
		takingDamageAnim
	};


	enum JumpState
	{
		ready,
		buttonPressed,
		inAir,
		landing
	};

	enum DamageState
	{
		normal,
		dead,
		takingDamage
	};


protected:

private:
	AnimationState m_animState;
	Direction m_inputDirection;
	JumpState m_jumpState;
	DamageState m_damageState;
	int m_jumpFrame;
	float m_jumpPower;
	/* gives an acceleration between minAcceleration and maxAcceleration depending on where value is between minValue and maxValue */
	static float calcAcceleration(float minAcceleration, float maxAcceleration, float useMinValue, float useMaxValue, float value);
	void jump();
	int m_hp;
};



#endif
