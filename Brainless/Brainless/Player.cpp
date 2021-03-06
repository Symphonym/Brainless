#include "Player.h"
#include "Constants.h"
#include "OptionsMenu.h"
#include "ParticleSystem.h"
#include <iostream>

/*
spring hopp 4 rutor /ej klar/
g� hopp 3 rutor
h�jd h�gt hopp 2 rutor
h�jd kort hopp 1 ruta
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
#define JUMPANIMFPS (float) 12 //hopp beroende p� denna, jumpPower skalas d�rf�r beroende p� den f�r att f� lika h�gt hopp

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
m_fallPos(startPosition.y),
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

void Player::setHealth(int health)
{
	m_hp = health;

	if (m_hp >= m_maxHp)
		m_hp = m_maxHp;
}

/*
Updates/executes all input and calculates new states
*/
void Player::updateTask(float deltaTime)
{
	if (!m_isMovementEnabled)
		return;


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
		if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Run"))) //Fortfarande inte riktigt bra, g�r fr�n maxrun till maxwalk direkt efter man sl�pper
		{
			run = true;
		}
		else run = false;

		bool slowDown = true;
		m_inputDirection = dir_noDirection;
		if (m_inAir) m_wallState = wall_normal; //annars st�rre problem med r�relsen i luften, men kan bli "minimala-sk�rm-studs" problem i luften ist�llet. Dock k�rs r�tt animation
		//Left
		if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Left")) && !sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Right"))/*&& m_wallState != wall_left*/)
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
		if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Right")) && !sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Left"))/*&& m_wallState != wall_right*/)
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
			if (30 < m_speed.y)m_jumpState = jump_inAir; //spelaren �r f�rmodligen inAir "p� riktigt", g�r ett fake hopp f�r att f� landanimation.
		}
		//If landed, can jump.
		else if (m_jumpState == jump_land)
		{
			if (m_animation.getPlayOnceDone()) m_jumpState = jump_ready;
		}
		//Pressing jump

		if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Jump")))
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
		if (!m_inAir) //kan inte r�ra p� dig f�rr�n du har landat
		{
			m_damageState = dmg_normal;
			m_speed.x = 0;
		}
		//Reset fall position to keep player from taking repeated damage from falling
		m_fallPos = getPosition().y;
	}
	else //dying
	{
		if (m_animState == anim_dead && m_animation.getPlayOnceDone())
		{
			m_acceleration = sf::Vector2f(0, 0);
			m_speed.x = 0;
		}
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && false)
	{
		m_damageState = dmg_normal;
		m_hp = 3;
	}
	if (m_inAir)
	{

		if (getPosition().y<m_fallPos)
			m_fallPos = getPosition().y;
	}
	else
	{
		if (getPosition().y - m_fallPos > Constants::TileSize * 7)
			takesDamage(m_position);
		m_fallPos = getPosition().y;
	}
}

int Player::getHealth()
{
	return m_hp;
}

int Player::getMaxHealth()
{
	return m_maxHp;
}

void Player::takesDamage(sf::Vector2f damageOrigin, int damage)
{
	if (!m_isMovementEnabled)
		return;
	if (m_damageState == dmg_normal)
	{
		m_hp -= damage;
		SoundPlayer::instance().playSound("player_hurt", getPosition());
		m_acceleration = sf::Vector2f(0, 0);
		if (0 < damageOrigin.x - m_position.x)
		{
			m_speed.x = -200;
			m_spriteDirection = dir_right;
		}
		else if (damageOrigin.x - m_position.x < 0)
		{
			m_speed.x = 200;
			m_spriteDirection = dir_left;
		}
		m_speed.y = -400;
		m_inAir = true;
		m_inTilt = false;
		//deny jump
		m_jumpPower = 0;
		m_jumpState = jump_ready;
		//specialSpriteDirection is set in animationUpdate
		
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
	m_cameraPos.x = m_position.x;
	m_cameraPos.y = m_position.y + cameraOffset;

	if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Down")) && m_speed.x == 0 && m_speed.y == 0)
	{
		cameraOffset = cameraOffset + (m_cameraMaxOffset - cameraOffset) * deltaTime * m_cameraSpeed;
	}
	else if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Up")) && m_speed.x == 0 && m_speed.y == 0)
	{
		cameraOffset = cameraOffset + (-m_cameraMaxOffset - cameraOffset) * deltaTime * m_cameraSpeed;
	}
	else
	{
		cameraOffset = cameraOffset + (0 - cameraOffset) * deltaTime * m_cameraSpeed;

		if (abs(m_cameraPos.y - m_position.y) < 2)
			cameraOffset = 0;
	}

	//breakpoint between idleJump/runJump animation
	float runJumpBreakpoint = MAX_WALK_SPEED_X;
	//breakpoint between walk/run animation
	float runBreakpoint = MAX_WALK_SPEED_X;

	m_specialSpriteDirection = false;

	/*
	Special State Conditions
	*/
	//Start to Land 
	if (!m_inAir && m_jumpState == jump_inAir)
	{
		m_jumpState = jump_land;
		SoundPlayer::instance().playSound("player_landing", getPosition());
		ParticleSystem::instance().addParticles(15, m_position + sf::Vector2f(20, 190), sf::Color(128, 128, 128), sf::Vector2f(0.4f, 0.4f), sf::Vector2f(0, 360), sf::Vector2f(0, 0), sf::Vector2f(-10, 0), sf::Vector2f(-10, 0), sf::Vector2f(-1, 0.5f));
		ParticleSystem::instance().addParticles(15, m_position + sf::Vector2f(60, 190), sf::Color(128, 128, 128), sf::Vector2f(0.4f, 0.4f), sf::Vector2f(0, 360), sf::Vector2f(0, 0), sf::Vector2f(-10, 0), sf::Vector2f(-10, 0), sf::Vector2f(1, 0.5f));
	}

	//Ladder Climbing
	if (m_climbing)
	{
		m_fallPos = getPosition().y;

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
			if (m_speed.y < -50)
			{
				//inAir
				if ((m_animState == anim_endJump && m_animation.getPlayOnceDone()) || m_animState == anim_inAirUpRun)
					animation_inAirUp();
				//endJump
				else if (m_animState != anim_inAirUp)
					animation_endJump();
			}
			//FALL
			else if (50 < m_speed.y)
			{
				animation_inAirFall();
			}
			//MIDAIR
			else
			{
				animation_inAir();
			}
		}
		//RUN JUMP
		else
		{
			//UP
			if (m_speed.y < -50)
				animation_inAirUpRun();
			//FALL
			else if (50 < m_speed.y)
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
	else if ((abs(m_speed.x) < 10))
		animation_idle();


	//RUN
	else if (runBreakpoint < abs(m_speed.x))
		animation_run();
	//WALK
	else if (10 < abs(m_speed.x))
		animation_walk();

	//else
	//{
	//	std::cout << "FIXA F�r ingen animation" << std::endl; //b�r inte uppst�
	//	std::cout << m_animState << std::endl;
	//}

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
"PlayerSheetRun" == 2 //Kommer senare f�rsvinna, just nu bara jumpRun
*/
void Player::animation_idle()
{
	if (m_animState != anim_idle)
	{
		m_sprite = &m_spriteSheets[0];
		m_animation.loop(0, 1, 3, 4, 8);
		m_animState = anim_idle;
	}
}

void Player::animation_walk()
{
	if (m_animState != anim_walk)
	{
		m_sprite = &m_spriteSheets[0];
		m_animation.loop(0, 7, 0, 5);
		m_animState = anim_walk;

	}
	m_animation.setSpeed(Animation::calcFrameSpeed(5, 10, 0, MAX_WALK_SPEED_X, abs(m_speed.x)));
}
void Player::animation_run()
{
	if (m_animState != anim_run)
	{
		m_sprite = &m_spriteSheets[0];
		m_animation.loop(0, 7, 2, 6);
		m_animState = anim_run;
	}
	m_animation.setSpeed(Animation::calcFrameSpeed(4, 10, MAX_WALK_SPEED_X, m_maxSpeed.x, abs(m_speed.x)));

}
void Player::animation_turn()
{
	if (m_animState != anim_turn)
	{
		m_sprite = &m_spriteSheets[0];
		m_animation.playOnce(0, 1, 5, 8);
		m_animState = anim_turn;
	}
}

void Player::animation_turnRun()
{
	if (m_animState != anim_turnRun)
	{
		m_sprite = &m_spriteSheets[2];
		m_animation.playOnce(0, 1, 5, 8);
		m_animState = anim_turnRun;
		SoundPlayer::instance().playSound("player_turn", getPosition(), 70.0f);

		if (getSpeed().x < 0)
		ParticleSystem::instance().addParticles(15, m_position + sf::Vector2f(-30, 190), sf::Color(128, 128, 128), sf::Vector2f(0.4f, 0.4f), sf::Vector2f(0, 360), sf::Vector2f(0, 0), sf::Vector2f(-50, 0), sf::Vector2f(-50, 0), sf::Vector2f(-3, 0.5f));
		else
			ParticleSystem::instance().addParticles(15, m_position + sf::Vector2f(130, 190), sf::Color(128, 128, 128), sf::Vector2f(0.4f, 0.4f), sf::Vector2f(0, 360), sf::Vector2f(0, 0), sf::Vector2f(-50, 0), sf::Vector2f(-50, 0), sf::Vector2f(3, 0.5f));

	}
}

void Player::animation_startJumpIdle()
{
	if (m_animState != anim_startJumpIdle)
	{
		m_sprite = &m_spriteSheets[1];
		m_animation.playOnce(0, 2, 0, JUMPANIMFPS);
		m_animState = anim_startJumpIdle;
	}
}
void Player::animation_startJumpRun()
{
	if (m_animState != anim_startJumpRun)
	{
		m_sprite = &m_spriteSheets[2];
		m_animation.playOnce(0, 1, 0, 8);
		m_animState = anim_startJumpRun;
	}
}
void Player::animation_endJump() //after leaving ground for "idlejump"
{
	if (m_animState != anim_endJump)
	{
		m_sprite = &m_spriteSheets[1];
		m_animation.playOnce(3, 5, 0, 12);
		m_animState = anim_endJump;
	}
}
void Player::animation_inAirUp()
{
	if (m_animState != anim_inAirUp)
	{
		m_sprite = &m_spriteSheets[1];
		m_animation.loop(0, 2, 1, 8);
		m_animState = anim_inAirUp;
	}
}
void Player::animation_inAirFall()
{
	if (m_animState != anim_inAirFall)
	{
		m_sprite = &m_spriteSheets[1];
		m_animation.loop(1, 3, 2, 8);
		m_animState = anim_inAirFall;
	}
}
void Player::animation_inAir()
{
	if (m_animState != anim_inAir)
	{
		m_sprite = &m_spriteSheets[1];
		m_animation.stillFrame(0, 2);
		m_animState = anim_inAir;
	}
}
void Player::animation_inAirUpRun()
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
}
void Player::animation_inAirRun()
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
		m_animation.playOnce(0, 1 + m_jumpFrame, 3, JUMPANIMFPS);
		m_animState = anim_landIdle;
		m_jumpFrame = 2;
	}
}
void Player::animation_landRun()
{
	if (m_animState != anim_landRun)
	{
		m_sprite = &m_spriteSheets[2];
		m_animation.playOnce(0, 1, 4, 8);
		m_animState = anim_landRun;
		m_jumpFrame = 2;
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
		m_sprite = &m_spriteSheets[2];
		m_animation.loop(0, 1, 6, 7, 6);
		m_animState = anim_climbingUp;
	}
}
void Player::animation_climbingDown()
{
	if (m_animState != anim_climbingDown)
	{
		m_sprite = &m_spriteSheets[2];
		m_animation.loop(0, 1, 6, 7, 6);
		m_animState = anim_climbingDown;
		m_animation.setReverse(true);
	}
}