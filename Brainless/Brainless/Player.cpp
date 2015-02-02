#include "Player.h"

#define MAX_SPEED_X 300
#define MAX_SPEED_Y 500
#define COLLISION_WIDTH 80
#define COLLISION_HEIGHT 190
#define SPRITE_OFFSET_X -85
#define SPRITE_OFFSET_Y -50

//m_size(sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT)),
//m_maxSpeed(sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y)),

//v0.02
Player::Player(sf::Vector2f startPosition)
:
Unit(startPosition, sf::Vector2f(COLLISION_WIDTH, COLLISION_HEIGHT), sf::Vector2f(MAX_SPEED_X, MAX_SPEED_Y), sf::Vector2f(SPRITE_OFFSET_X, SPRITE_OFFSET_Y)),
m_state(noAnimation),
m_spriteDirection(right),
m_inputDirection(noDirection),
m_jumpState(ready)
{

}

void Player::checkPlayerInput()
{
	float speedTurnAround = 12;
	float speedStartAcc = 500;
	float speedNormalAcc = 200;
	float speedSlowDown = 6;

	float startAccBreakpoint = 150;
	float minSpeedBeforeStop = 10;

	bool slowDown = true;
	m_inputDirection = noDirection;
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
		m_inputDirection = left;
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
		m_inputDirection = right;
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
	//Jump
	if (!m_inAir) m_jumpState = ready; // can jump
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{

		if (m_jumpState != buttonReleased)
		{

			//jump while in the air
			if (m_inAir)
			{
				if (m_speed.y < -200)
					m_acceleration.y = -1 * calcAcceleration(0, 500, 0, 300, -1 * m_speed.y);
				else m_acceleration.y = 0;
				// vid speedY ca 200
			}
			//from ground
			else
			{
				m_speed.y = -300;
				m_inAir = true;
				m_jumpState = buttonPressed;
			}
		}
		else //stop jump
		{
			m_acceleration.y = 0;
		}
	}
	//prevent "re-jump" after releasing button
	else if (m_jumpState == buttonPressed)
	{
		m_jumpState = buttonReleased;
	}
	//stop the jump
	else
	{
		m_acceleration.y = 0;
	}

}

void Player::updateAnimation(float deltaTime)
{
	float runBreakpoint = m_maxSpeed.x * 2 / 3;



	if (m_inAir)
	{
		//JUMP
		if (m_speed.y < 0)
		{
			if (m_state != jump)
			{
				m_sprite = &m_spritSheets[0];
				m_animation.loop(0, 3, 7, 5);
				m_state = jump;
			}
		}
		//FALL
		else if (0 <= m_speed.y)
		{
			if (m_state != fall)
			{
				m_sprite = &m_spritSheets[0];
				m_animation.loop(0, 3, 7, 5);
				m_state = fall;
			}
		}
	}
	//IDLE
	else if (m_speed.x == 0)
	{
		if (m_state != idle)
		{
			m_sprite = &m_spritSheets[0];
			m_animation.stillFrame(0, 3);
			m_state = idle;
		}
	}
	//TURN
	else if (m_speed.x < 0 && m_inputDirection == right || 0 < m_speed.x && m_inputDirection == left)
	{
		if (m_state != turn)
		{
			m_sprite = &m_spritSheets[0];
			m_animation.playOnce(0, 3, 5, 10);
			m_state = turn;
		}
	}
	//RUN
	//else if (runBreakpoint <= m_speed.x || m_speed.x <= -runBreakpoint)
	else if (m_state == run  || ((m_state == startWalk || m_state == endWalk) && m_animation.getPlayOnceDone()))
	{
		if (m_state != run)
		{
			m_sprite = &m_spritSheets[0];
			m_animation.loop(0, 7, 1, 10);
			m_state = run;
		}
		m_animation.setSpeed(Animation::calcFrameSpeed(10, 15, runBreakpoint, m_maxSpeed.x, abs(m_speed.x)));

	}
	//START WALK
	else if (m_speed.x != 0 && (m_inputDirection == right || m_inputDirection == left))
	{
		if (m_state != startWalk)
		{
			m_sprite = &m_spritSheets[0];
			m_animation.playOnce(0, 3, 0, 8);
			m_state = startWalk;
		}
	//	m_animation.setSpeed(Animation::calcFrameSpeed(5, 20, 0, runBreakpoint, abs(m_speed.x)));
	}
	//END WALK
	else if (m_speed.x != 0)
	{
		if (m_state != endWalk)
		{
			m_sprite = &m_spritSheets[0];
			m_animation.playOnce(0, 3, 2, 8);
			m_state = endWalk;
		}
	//	m_animation.setSpeed(Animation::calcFrameSpeed(5, 20, 0, runBreakpoint, abs(m_speed.x)));
	}
	//TODO
	//startJump
	//endJump


	//Sprite mirroring and offset.
	if (m_spriteDirection == left)
	{
		if (0 < m_speed.x)
		{
			m_spriteDirection = right;
			m_sprite->setScale(1, 1);
			m_sprite->setPosition(sf::Vector2f(m_position.x + m_spriteOffset.x, m_position.y + m_spriteOffset.y + m_spriteOffset.y));

		}
		m_sprite->setScale(-1, 1);
		m_sprite->setPosition(m_position.x + m_spriteOffset.x + m_animation.getWidth(), m_position.y + m_spriteOffset.y);
	}
	else if (m_spriteDirection == right)
	{
		if (m_speed.x < 0)
		{
			m_spriteDirection = left;
			m_sprite->setScale(-1, 1);
			m_sprite->setPosition(m_position.x + m_spriteOffset.x + m_animation.getWidth(), m_position.y + m_spriteOffset.y);
		}
		m_sprite->setScale(1, 1);
		m_sprite->setPosition(sf::Vector2f(m_position.x + m_spriteOffset.x, m_position.y + m_spriteOffset.y));
	}



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
