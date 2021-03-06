#ifndef _PLAYER
#define _PLAYER

#include "Unit.h"
#include "Item.h"
//version 0.2
class Player : public Unit
{
public:
	Player(sf::Vector2f startPosition);
	
	
	void setClimbing(bool climbing);

	/* kollar efter playerinput, ska k�ras innan Unit::updateMovement */
	void updateTask(float deltaTime);
	//virtual void onCollideWithItem(Item &item) { item.flyOff(); };

	void updateAnimation(float deltaTime);

	sf::Vector2f getCameraPosition();

	void takesDamage(sf::Vector2f collisionPos, int damage = 1);

	void setHealth(int health);

	int getHealth();
	int getMaxHealth();

	enum AnimationState
	{
		anim_noAnimation,
		anim_idle,

		anim_walk,
		anim_run,
		anim_turnRun,
		anim_turn,

		anim_startJumpIdle,
		anim_startJumpRun,
		anim_endJump,

		anim_inAirUp,
		anim_inAirFall,
		anim_inAir,
		anim_inAirUpRun,
		anim_inAirFallRun,
		anim_inAirRun,

		anim_landIdle,
		anim_landRun,

		anim_dead,
		anim_damaged,

		anim_climbingUp,
		anim_climbingDown,
	};


	enum JumpState
	{
		jump_ready,
		jump_buttonPressed,
		jump_inAir,
		jump_land
	};

	enum DamageState
	{
		dmg_normal,
		dmg_dead,
		dmg_damaged
	};

	void wallLeft();
	void wallRight();

protected:

private:
	enum WallState
	{
		wall_normal,
		wall_left,
		wall_right
	};
	WallState m_wallState;
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
	const int m_maxHp = 3;
	float m_fallPos; //Indicating how long the Unit has been in the air

	sf::Vector2f m_cameraPos;
	float cameraOffset;
	float m_cameraSpeed = 5;
	float m_cameraMaxOffset = 250;
	int m_animLoopsDone = 0;

	bool m_climbing;

	/*
		animationFuncs
		will play last used animation Function until a new one is called, calling the same animation multiple times in a row will do nothing.
	*/
	void animation_idle();
	void animation_walk();
	void animation_run();
	void animation_turn();
	void animation_turnRun();

	void animation_startJumpIdle();
	void animation_startJumpRun();
	void animation_endJump();

	void animation_inAirUp();
	void animation_inAirFall();
	void animation_inAir();
	void animation_inAirUpRun();
	void animation_inAirFallRun();
	void animation_inAirRun();

	void animation_landIdle();
	void animation_landRun();

	void animation_dead();
	void animation_damaged();

	void animation_climbingUp();
	void animation_climbingDown();
};



#endif
