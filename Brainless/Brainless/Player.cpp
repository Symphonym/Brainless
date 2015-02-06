#include "Player.h"
#include <iostream>

#define MAX_SPEED_X (float) 300
#define MAX_SPEED_Y (float) 700
#define COLLISION_WIDTH (int) 80
#define COLLISION_HEIGHT (int) 190
#define SPRITE_OFFSET_X (int) -85
#define SPRITE_OFFSET_Y (int) -50
#define JUMP_POWER (float) 400
#define JUMP_POWER_PER_SEC (float) 200
#define JUMPANIMFPS (float) 12 //hopp beroende p� denna, jumpPower skalas d�rf�r beroende p� den f�r att f� lika h�gt hopp

//m_size(sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT)),
//m_maxSpeed(sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y)),

//v0.02
Player::Player(sf::Vector2f startPosition)
:
Unit(startPosition, sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT), sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y), sf::Vector2f(SPRITE_OFFSET_X, SPRITE_OFFSET_Y), ID_Player),
m_animState(anim_noAnimation),
m_inputDirection(dir_noDirection),
m_jumpState(jump_ready),
m_jumpPower(0),
m_jumpFrame(2),
m_hp(3),
m_damageState(dmg_normal)
{
	m_cameraPos = m_position;
}

void Player::setClimbing(bool climbing)
{
	m_climbing = climbing;
	m_isMovementEnabled = !climbing;
}

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

	//startpos + (endpos - startpos)*u;

	if (m_damageState == dmg_normal)
	{
		float speedTurnAround = 12;
		float speedStartAcc = 500;
		float speedNormalAcc = 200;
		float speedSlowDown = 6;

		float startAccBreakpoint = 150;
		float minSpeedBeforeStop = 10;



		bool slowDown = true;
		m_inputDirection = dir_noDirection;
		//Left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			if (20 < m_speed.x) //wrong direcion - slow character
			{
				m_acceleration.x = -m_speed.x * speedTurnAround;
			}
			else
			{
				if (abs(m_speed.x) < startAccBreakpoint)
					m_acceleration.x = -speedStartAcc;
				else
					m_acceleration.x = -speedNormalAcc;
			}

			slowDown = false;
			m_inputDirection = dir_left;
		}
		//Right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			if (m_speed.x < -20) //wrong direcion - slow character
			{
				m_acceleration.x = -m_speed.x * speedTurnAround;
			}
			else
			{
				if (abs(m_speed.x) < startAccBreakpoint)
					m_acceleration.x = speedStartAcc;
				else
					m_acceleration.x = speedNormalAcc;
			}
			slowDown = false;
			m_inputDirection = dir_right;
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
		/* Jump states and conditions */
		//in Air
		if (m_inAir)
		{
			//"cliffjump"
			if (m_jumpState == jump_buttonPressed)
			{
				jump();
			}
			if (30 < m_speed.y)m_jumpState = jump_inAir; //spelaren �r f�rmodligen inAir p� "riktigt", faka ett hopp f�r att f� landanimation.
		}
		//Start to Land
		else if (!m_inAir && m_jumpState == jump_inAir) m_jumpState = jump_land;
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
				m_jumpPower += deltaTime / (3 / JUMPANIMFPS) * JUMP_POWER_PER_SEC; //magic number, beroende p� animation just nu
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
		//if (m_animState == takingDamageAnim && m_animation.getPlayOnceDone())
		if (!m_inAir)
		{
			m_damageState = dmg_normal;
			m_speed.x = 0;
		}
		//g�r cool pushback r�relse
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
	if (m_damageState == dmg_normal)
	{
		m_hp--;
		
		m_acceleration = sf::Vector2f(0, 0);
		if (0 < collisionDifference.x) m_speed.x = -200;
		else m_speed.x = 200;
		m_speed.y = -400;
		m_inAir = true;
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
	m_speed.y = -m_jumpPower;
	m_jumpState = jump_inAir;
	m_inAir = true;
	
}

sf::Vector2f Player::getCameraPosition()
{
	return m_cameraPos;
}

void Player::updateAnimation(float deltaTime)
{
	float runBreakpoint = m_maxSpeed.x * 2 / 3; //anv�nds nu endast f�r att snabba upp run animation beroende p� maxSpeed och runBreakpoint

	m_specialSpriteDirection = false;

	//Dead
	if (m_damageState == dmg_dead)
	{
		m_specialSpriteDirection = true;
		if (m_animState != anim_dead)
		{
			m_sprite = &m_spriteSheets[0];
			m_animation.playOnce(0, 4, 6, 5);
			m_animState = anim_dead;
		}
	}
	//TakingDamage
	else if (m_damageState == dmg_damaged)
	{
		m_specialSpriteDirection = true;
		if (m_animState != anim_damaged)
		{
			m_sprite = &m_spriteSheets[0];
			m_animation.playOnce(0, 1, 6, 2);
			m_animState = anim_damaged;
		}
	}
	//startJump
	else if (m_jumpState == jump_buttonPressed)
	{
		if (m_animState != anim_startJump)
		{
			m_sprite = &m_spriteSheets[1];
			m_animation.playOnce(0, 2, 0, JUMPANIMFPS);
			m_animState = anim_startJump;
		}
	}
	//land
	else if (m_jumpState == jump_land)
	{
		if (m_animState != anim_land)
		{
			m_sprite = &m_spriteSheets[1];
			m_animation.playOnce(0, 1 + m_jumpFrame, 3, JUMPANIMFPS); //jumpFrame = experimental 3
			m_animState = anim_land;
			m_jumpFrame = 2; //experimental 
		}
	}
	else if (m_inAir)
	{
		//JUMP
		if (m_speed.y < -30)
		{

			//inAir
			if (m_animState == anim_inAirUp);
			else if (m_animState == anim_endJump && m_animation.getPlayOnceDone())
			{
				m_sprite = &m_spriteSheets[1];
				m_animation.loop(0, 1, 1, 5);
				m_animState = anim_inAirUp;
			}
			//endJump
			else if (m_animState != anim_endJump)
			{
				m_sprite = &m_spriteSheets[1];
				m_animation.playOnce(3, 5, 0, 10);
				m_animState = anim_endJump;

			}
		}
		//FALL
		else if (30 < m_speed.y)
		{
			if (m_animState != anim_inAirFall)
			{
				m_sprite = &m_spriteSheets[1];
				m_animation.loop(0, 1, 2, 6);
				m_animState = anim_inAirFall;
			}
		}
	}
	//IDLE
	else if ((-5 < m_speed.x && m_speed.x < 5 ))
	{
		if (m_animState != anim_idle)
		{
			m_sprite = &m_spriteSheets[0];
			m_animation.stillFrame(0, 3);
			m_animState = anim_idle;
		}
	}
	//TURN
	else if (m_speed.x < 0 && m_inputDirection == dir_right || 0 < m_speed.x && m_inputDirection == dir_left)
	{
		if (m_animState != anim_turn)
		{
			m_sprite = &m_spriteSheets[0];
			m_animation.playOnce(0, 3, 5, 10);
			m_animState = anim_turn;
		}
	}
	//RUN
	//else if (runBreakpoint <= m_speed.x || m_speed.x <= -runBreakpoint)
	else if (m_animState == anim_run || ((m_animState == anim_startWalk || m_animState == anim_endWalk) && m_animation.getPlayOnceDone()))
	{
		if (m_animState != anim_run)
		{
			m_sprite = &m_spriteSheets[0];
			m_animation.loop(0, 7, 1, 8);
			m_animState = anim_run;
		}
		m_animation.setSpeed(Animation::calcFrameSpeed(8, 12, runBreakpoint, m_maxSpeed.x, abs(m_speed.x)));

	}
	//START WALK
	else if ((m_speed.x < -5 || 5 < m_speed.x) && (m_inputDirection == dir_right || m_inputDirection == dir_left))
	{
		if (m_animState != anim_startWalk)
		{
			m_sprite = &m_spriteSheets[0];
			m_animation.playOnce(0, 3, 0, 8);
			m_animState = anim_startWalk;
		}
		//	m_animation.setSpeed(Animation::calcFrameSpeed(5, 20, 0, runBreakpoint, abs(m_speed.x)));
	}
	//END WALK
	else if ((m_speed.x < -5 || 5 < m_speed.x))
	{
		if (m_animState != anim_endWalk)
		{
			m_sprite = &m_spriteSheets[0];
			m_animation.playOnce(0, 3, 2, 8);
			m_animState = anim_endWalk;
		}
		//	m_animation.setSpeed(Animation::calcFrameSpeed(5, 20, 0, runBreakpoint, abs(m_speed.x)));
	}
	else 
	{
		std::cout << "FIXA F�r ingen animation" << std::endl; //b�r inte uppst�, har hittils inte uppst�tt
	}

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
