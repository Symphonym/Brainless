#include "Player.h"
#include <iostream>

/*
	spring hopp 4 rutor /ej klar/
	gå hopp 3 rutor
	höjd högt hopp 2 rutor
	höjd kort hopp 1 ruta
*/
#define MAX_SPEED_X (float) 300
#define MAX_WALK_SPEED_X (float) 200
#define MAX_SPEED_Y (float) 700
#define COLLISION_WIDTH (int) 80
#define COLLISION_HEIGHT (int) 190
#define SPRITE_OFFSET_X (int) -85
#define SPRITE_OFFSET_Y (int) -50
#define JUMP_POWER (float) 400
#define JUMP_POWER_PER_SEC (float) 200
#define JUMPANIMFPS (float) 12 //hopp beroende på denna, jumpPower skalas därför beroende på den för att få lika högt hopp

//v0.02
Player::Player(sf::Vector2f startPosition)
:
Unit(startPosition, sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT), sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y), sf::Vector2f(SPRITE_OFFSET_X, SPRITE_OFFSET_Y), ID_Player),
m_animState(anim_noAnimation),
m_inputDirection(dir_noDirection),
m_jumpState(jump_ready),
m_jumpPower(0),
m_jumpFrame(2),
m_wallState(wall_normal),
m_hp(3),
m_damageState(dmg_normal)
{
	m_cameraPos = m_position;
	s_playerPointer = this;
}

void Player::setClimbing(bool climbing)
{
	m_climbing = climbing;
	m_isMovementEnabled = !climbing;
}

/*
	Updates/executes all input and calculates new states
*/
void Player::updateTask(float deltaTime)
{
	m_cameraPos.x = m_position.x;
	m_cameraPos.y = m_position.y + cameraOffset;

	if (!m_isMovementEnabled)
		return;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && m_speed.x == 0 && m_speed.y == 0)
	{
		cameraOffset = cameraOffset + (m_cameraMaxOffset - cameraOffset) * deltaTime * m_cameraSpeed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && m_speed.x == 0 && m_speed.y == 0)
	{
		cameraOffset = cameraOffset + (-m_cameraMaxOffset - cameraOffset) * deltaTime * m_cameraSpeed;
	}
	else
	{
		cameraOffset = cameraOffset + (0 - cameraOffset) * deltaTime * m_cameraSpeed;

		if (abs(m_cameraPos.y - m_position.y) < 2)
			cameraOffset = 0;
	}

	if (m_damageState == dmg_normal)
	{
	
		/*
			Walk/Run states
		*/
		float speedTurnAround = 8;
		float speedStartAcc = 500;
		float speedNormalAcc = 200;
		float speedSlowDown = 5;

		float startAccBreakpoint = 100;
		float minSpeedBeforeStop = 10;
		bool run = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) //Fortfarande inte riktigt bra, går från maxrun till maxwalk direkt efter man släpper
		{
			run = true;
		}
		else run = false;

		bool slowDown = true;
		m_inputDirection = dir_noDirection;
		if (m_inAir) m_wallState = wall_normal; //annars större problem med rörelsen i luften, men kan bli "minimala-skärm-studs" problem i luften istället. Dock körs rätt animation
		//Left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) /*&& m_wallState != wall_left*/)
		{
			slowDown = false;
			m_inputDirection = dir_left;
			if (20 < m_speed.x) //wrong direcion - slow character
			{
				m_acceleration.x = -m_speed.x * speedTurnAround;
			}
			else if (!run && MAX_WALK_SPEED_X < abs(m_speed.x) + 20) //+20 ful hotfix, ex. if speed = 280, += 22 => 302 speed -> error i animation
			{
				//slowDown = true;
				m_speed.x = -MAX_WALK_SPEED_X;
				m_acceleration.x = 0;
			}
			else
			{
				m_wallState = wall_normal;
				if (abs(m_speed.x) < startAccBreakpoint)
					m_acceleration.x = -speedStartAcc;
				else
				{
					m_wallState = wall_normal;
					m_acceleration.x = -speedNormalAcc;
				}
					
			}

		}
		//Right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) /*&& m_wallState != wall_right*/)
		{
			slowDown = false;
			m_inputDirection = dir_right;
			if (m_speed.x < -20) //wrong direcion - slow character
			{
				m_acceleration.x = -m_speed.x * speedTurnAround;
			}
			else if (!run && MAX_WALK_SPEED_X < abs(m_speed.x) + 20) //+20 ful hotfix, ex. if speed = 280, += 22 => 302 speed -> error i animation
			{
				//slowDown = true;
				m_speed.x = MAX_WALK_SPEED_X;
				m_acceleration.x = 0;
			}
			else
			{
				
				if (abs(m_speed.x) < startAccBreakpoint)
					m_acceleration.x = speedStartAcc;
				else
				{
					m_wallState = wall_normal;
					m_acceleration.x = speedNormalAcc;
				}
			}

		}




		if (slowDown)
		{
			//small values = stop totally
			if (m_speed.x < minSpeedBeforeStop && m_speed.x > -minSpeedBeforeStop)
			{
				m_speed.x = 0;
				m_acceleration.x = 0;
			}
			//slow
			else m_acceleration.x = -m_speed.x * speedSlowDown;
		}
		
		/* 
			Jump states and conditions 
		*/
		//in Air
		if (m_inAir)
		{
			//"cliffjump"
			if (m_jumpState == jump_buttonPressed)
			{
				jump();
			}
			if (30 < m_speed.y)m_jumpState = jump_inAir; //spelaren är förmodligen inAir "på riktigt", gör ett fake hopp för att få landanimation.
		}
		//Start to Land
		else if (!m_inAir && m_jumpState == jump_inAir)
		{
			m_jumpState = jump_land;
			SoundPlayer::instance().playSound("player_landing", getPosition());
		}
		//If landed, can jump.
		else if (m_jumpState == jump_land)
		{
			if (m_animation.getPlayOnceDone()) m_jumpState = jump_ready;
		}
		//Pressing jump

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			//don't jump
			if (m_inAir)
			{
				m_jumpPower = 0;
				m_jumpState = jump_inAir;
			}
			//can jump
			else if (m_jumpState == jump_ready)
			{

				//from ground
				m_jumpPower = JUMP_POWER;
				m_jumpState = jump_buttonPressed;
			}
			//has started to jump
			else if (m_jumpState == jump_buttonPressed)
			{
				m_jumpPower += deltaTime / (3 / JUMPANIMFPS) * JUMP_POWER_PER_SEC; //magic number, beroende på animation just nu
				//force jump
				if (m_animation.getPlayOnceDone())
				{
					jump();
				}
			}
		}

		//releasing jump button
		else if (m_jumpState == jump_buttonPressed)
		{
			jump();
		}

	}
	else if (m_damageState == dmg_damaged)
	{
		if (!m_inAir) //kan inte röra på dig förrän du har landat
		{
			m_damageState = dmg_normal;
			m_speed.x = 0;
		}
	}
	else //dying
	{
		if (m_animState == anim_dead && m_animation.getPlayOnceDone())
		{
			m_acceleration = sf::Vector2f(0, 0);
			m_speed.x = 0;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		m_damageState = dmg_normal;
		m_hp = 3;
	}
}

void Player::takesDamage(sf::Vector2f collisionDifference)
{
	if (!m_isMovementEnabled)
		return;
	if (m_damageState == dmg_normal)
	{
		m_hp--;
		SoundPlayer::instance().playSound("player_hurt", getPosition());
		m_acceleration = sf::Vector2f(0, 0);
		if (0 < collisionDifference.x) m_speed.x = -200;
		else m_speed.x = 200;
		m_speed.y = -400;
		m_inAir = true;
		m_inTilt = false;
		//deny jump
		m_jumpPower = 0;
		m_jumpState = jump_ready;
		//specialSpriteDirection is set in animationUpdate
		if (0 < collisionDifference.x) m_spriteDirection = dir_right;
		else m_spriteDirection = dir_left;

		if (0 < m_hp)
		{
			m_damageState = dmg_damaged;
		}
		else
		{
			m_damageState = dmg_dead;
		}
	}
}

/* jumps based off m_jumpPower */
void Player::jump()
{
	if (m_maxSpeed.y < m_jumpPower) m_jumpPower = m_maxSpeed.y;
	m_jumpFrame = m_animation.getCurrentFrame(); //experimental
	m_acceleration.y = -m_jumpPower*20.f;
	//m_speed.y = -m_jumpPower;
	m_jumpState = jump_inAir;
	m_inAir = true;
	m_inTilt = false;
	SoundPlayer::instance().playSound("player_jump", getPosition());
}

void Player::wallLeft()
{
	m_wallState = wall_left;
}
void Player::wallRight()
{
	m_wallState = wall_right;
}
sf::Vector2f Player::getCameraPosition()
{
	return m_cameraPos;
}

/* 
	Determines which animations should be called
*/
void Player::updateAnimation(float deltaTime)
{
		
	//breakpoint between idleJump/runJump animation
	float runJumpBreakpoint = MAX_WALK_SPEED_X;
	//breakpoint between walk/run animation
	float runBreakpoint = MAX_WALK_SPEED_X;

	m_specialSpriteDirection = false;

	/*
		Special State Conditions
	*/

	//Ladder Climbing
	if (m_climbing)
	{
		if (m_speed.y == -1)
			animation_climbingUp();

		else if (m_speed.y == 1) 
			animation_climbingDown();
	}
	//Dead
	else if (m_damageState == dmg_dead)
		animation_dead();

	//TakingDamage
	else if (m_damageState == dmg_damaged)
		animation_damaged();
	
	//startJump
	else if (m_jumpState == jump_buttonPressed)
	{
		if (m_animState == anim_startJumpIdle || m_animState == anim_startJumpRun);

		else if (abs(m_speed.x) <= runBreakpoint) //runJumpBreakpoint
			animation_startJumpIdle();

		else
			animation_startJumpRun();
	}
	//land
	else if (m_jumpState == jump_land)
	{
		if (m_animState == anim_landIdle || m_animState == anim_landRun);
		
		else if (abs(m_speed.x) <= runBreakpoint) //runJumpBreakpoint
			animation_landIdle();
		
		else
			animation_landRun();
		
	}
	else if (m_inAir && !m_inTilt)
	{
		//"IDLE"JUMP
		if (abs(m_speed.x) <= runBreakpoint) //runJumpBreakpoint
		{
			//UP
			if (m_speed.y < 30)
			{
				//inAir
				if ((m_animState == anim_endJump && m_animation.getPlayOnceDone()) || m_animState == anim_inAirUpRun)
					animation_inAirUp();

				//endJump
				else if (m_animState != anim_inAirUp)
					animation_endJump();
			}
			//FALL
			else if (30 < m_speed.y)
			{
				animation_inAirFall();
			}
		}
		//RUN JUMP
		else
		{
			//UP
			if (m_speed.y < -100)
				animation_inAirUpRun();
			//FALL
			else if (100 < m_speed.y)
				animation_inAirFallRun();
			//MIDAIR
			else
				animation_inAirRun();


		}
	}


	//TURN
	else if (m_speed.x < 0 && m_inputDirection == dir_right || 0 < m_speed.x && m_inputDirection == dir_left)
		//FAST
	if (m_animState == anim_run || m_animState == anim_turnRun)
		animation_turnRun();
	//SLOW
	else
		animation_turn();

	//IDLE
	else if ((abs(m_speed.x) < 5))
		animation_idle();


	//RUN
	else if (runBreakpoint < abs(m_speed.x))
		animation_run();
	//WALK
	else if (5 < abs(m_speed.x))
		animation_walk();
	//
	////TURN
	//else if (m_speed.x < -5 && m_inputDirection == dir_right || 5 < m_speed.x && m_inputDirection == dir_left)
	//	//FAST
	//if (m_animState == anim_run || m_animState == anim_turnRun)
	//	animation_turnRun();
	////SLOW
	//else
	//	animation_turn();

	////IDLE
	//else if ((abs(m_speed.x) < 5) && (m_inputDirection == dir_noDirection || m_animState == anim_turn || m_animState == anim_turnRun))
	//	animation_idle();

	////RUN
	//else if (runBreakpoint < abs(m_speed.x))
	//	animation_run();
	////WALK
	//else if (5 < abs(m_speed.x))
	//	animation_walk();

	else 
	{
		std::cout << "FIXA Får ingen animation" << std::endl; //bör inte uppstå
		std::cout << m_animState << std::endl;
	}

	//if(m_animState == 8)std::cout << "blah" << std::endl;

	updateSpriteDirection();
	m_sprite->setTextureRect(m_animation.getRectangle(deltaTime));
}

/*
returns acceleration closer minAcceleration when value is closer to useMinValue
returns acceleration closer maxAcceleration when value is closer to useMaxValue
useMinValue can be higher than useMaxValue
*/
float Player::calcAcceleration(float minAcceleration, float maxAcceleration, float useMinValue, float useMaxValue, float value)
{
	if (minAcceleration == maxAcceleration) return minAcceleration;

	//"Reverted"
	if (useMaxValue < useMinValue)
	{
		if (value < useMaxValue) return maxAcceleration;
		else if (useMinValue < value) return minAcceleration;
		float b = useMinValue - useMaxValue;
		float a = abs(value) - useMaxValue;
		if (a == 0) return maxAcceleration;
		return maxAcceleration - (maxAcceleration - minAcceleration)*(b / a);
	}
	//Normal
	if (useMaxValue < value) return maxAcceleration;
	else if (value < useMinValue) return minAcceleration;
	float a = useMaxValue - useMinValue;
	float b = abs(value) - useMinValue;
	if (a == 0) return minAcceleration;
	return minAcceleration + (maxAcceleration - minAcceleration)*(b / a);
}

/*
	All animation calls
	"PlayerSheet" == 0
	"PlayerSheetJump" == 1
	"PlayerSheetRun" == 2
*/
void Player::animation_idle()
{
	if (m_animState != anim_idle)
	{
	//	m_animLoopsDone = 0;
		m_sprite = &m_spriteSheets[0];
	
		m_animation.loop(0, 1, 3, 4, 8);

		m_animState = anim_idle;
	}

	/*if (m_animation.getPlayOnceDone()) //rip kevins glada smiley, death by grafikernas nya textur
	{
		m_animLoopsDone++;
		if (m_animLoopsDone == 8)
		{
			m_animation.playOnce(0, 7, 4, 8);
		}
		else
		m_animation.playOnce(0, 7, 3, 8);
	}*/
}

void Player::animation_walk()
{
	//std::cout << "hej2" << std::endl;
	//std::cout << m_animState << std::endl;
	if (m_animState != anim_walk)
	{
		//std::cout << "hej" << std::endl;
		m_sprite = &m_spriteSheets[0];
		m_animation.loop(0, 7, 1, 5);
		m_animState = anim_walk;
	}
	m_animation.setSpeed(Animation::calcFrameSpeed(5, 10, 0, MAX_WALK_SPEED_X, abs(m_speed.x)));
}
void Player::animation_run()
{
	if (m_animState != anim_run)
	{
		m_sprite = &m_spriteSheets[2];
		m_animation.loop(0, 7, 5, 6);
		m_animState = anim_run;
	}
	m_animation.setSpeed(Animation::calcFrameSpeed(4, 10, MAX_WALK_SPEED_X, m_maxSpeed.x, abs(m_speed.x))); //runBreakpoint används inte på samma sätt, vänta tills vet hur run/walk ska funka

}
void Player::animation_turn()
{
	if (m_animState != anim_turn)
	{
		m_sprite = &m_spriteSheets[0];
		m_animation.playOnce(2, 3, 5, 8);
		m_animState = anim_turn;
	}
}

void Player::animation_turnRun()
{
	if (m_animState != anim_turnRun)
	{
		m_sprite = &m_spriteSheets[2];
		m_animation.playOnce(0, 1, 6, 8);
		m_animState = anim_turnRun;
		SoundPlayer::instance().playSound("player_turn", getPosition());
	}
}

void Player::animation_startJumpIdle()
{
	if (m_animState != anim_startJumpIdle)
	{
		m_sprite = &m_spriteSheets[1];
		m_animation.playOnce(0, 2, 0, JUMPANIMFPS);
		m_animState = anim_startJumpIdle;
		//std::cout << "JumpIdle" << std::endl;
		//std::cout << m_speed.x << std::endl;
	}
}
void Player::animation_startJumpRun()
{
	if (m_animState != anim_startJumpRun)
	{
		m_sprite = &m_spriteSheets[2];
		m_animation.playOnce(0, 1, 0, 8);
		m_animState = anim_startJumpRun;
		//std::cout << "JumpRun" << std::endl;
		//std::cout << m_speed.x << std::endl;
	}
}
void Player::animation_endJump() //after leaving ground for "idlejump"
{
	if (m_animState != anim_endJump)
	{
		m_sprite = &m_spriteSheets[1];
		m_animation.playOnce(3, 5, 0, 10);
		m_animState = anim_endJump;
	}
}
void Player::animation_inAirUp()
{
	if (m_animState != anim_inAirUp)
	{
		m_sprite = &m_spriteSheets[1];
		m_animation.loop(0, 1, 1, 6);
		m_animState = anim_inAirUp;
	}
}
void Player::animation_inAirFall()
{
	if (m_animState != anim_inAirFall)
	{
		m_sprite = &m_spriteSheets[1];
		m_animation.loop(0, 1, 2, 6); 
		m_animState = anim_inAirFall;
	}
}void Player::animation_inAirUpRun()
{
	if (m_animState != anim_inAirUpRun)
	{
		m_sprite = &m_spriteSheets[2];
		m_animation.loop(0, 1, 1, 6); //
		m_animState = anim_inAirUpRun;
	}
}
void Player::animation_inAirFallRun()
{
	if (m_animState != anim_inAirFallRun)
	{
		m_sprite = &m_spriteSheets[2];
		m_animation.loop(0, 1, 3, 6); //
		m_animState = anim_inAirFallRun;
	}
}void Player::animation_inAirRun()
{
	if (m_animState != anim_inAirRun)
	{
		m_sprite = &m_spriteSheets[2];
		m_animation.loop(0, 1, 2, 6); //
		m_animState = anim_inAirRun;
	}
}
void Player::animation_landIdle()
{
	if (m_animState != anim_landIdle)
	{
		m_sprite = &m_spriteSheets[1];
		m_animation.playOnce(0, 1 + m_jumpFrame, 3, JUMPANIMFPS); //jumpFrame = experimental 3
		m_animState = anim_landIdle;
		m_jumpFrame = 2; //experimental 
		//std::cout << "LandIdle" << std::endl;
		//std::cout << m_speed.x << std::endl;
	}
}
void Player::animation_landRun()
{
	if (m_animState != anim_landRun)
	{
		m_sprite = &m_spriteSheets[2];
		m_animation.playOnce(0, 1, 4, 8); //jumpFrame = experimental 3
		m_animState = anim_landRun;
		m_jumpFrame = 2; //experimental 
	/*	std::cout << "LandRun" << std::endl;
		std::cout << m_speed.x << std::endl;*/
	}
}
void Player::animation_dead()
{
	m_specialSpriteDirection = true;
	if (m_animState != anim_dead)
	{
		m_sprite = &m_spriteSheets[0];
		m_animation.playOnce(0, 4, 6, 5);
		m_animState = anim_dead;
	}
}
void Player::animation_damaged()
{
	m_specialSpriteDirection = true;
	if (m_animState != anim_damaged)
	{
		m_sprite = &m_spriteSheets[0];
		m_animation.playOnce(0, 1, 6, 2);
		m_animState = anim_damaged;
	}
}
void Player::animation_climbingUp()
{
	if (m_animState != anim_climbingUp)
	{
		m_sprite = &m_spriteSheets[0];
		m_animation.loop(0, 3, 7, 6);
		m_animState = anim_climbingUp;
	}
}
void Player::animation_climbingDown()
{
	if (m_animState != anim_climbingDown)
	{
		m_sprite = &m_spriteSheets[0];
		m_animation.loop(0, 3, 7, 6);
		m_animState = anim_climbingDown;
		m_animation.setReverse(true);
	}
}