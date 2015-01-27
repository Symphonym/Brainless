#include "Animation.h"

Animation::Animation(int width, int height)
:
m_width(width),
m_height(height),
m_rectangle(0, 0, width, height)
{
}


void Animation::loop(int startFrame, int endFrame, int frameRow, float speed)
{
	m_startFrame = startFrame;
	m_endFrame = endFrame;
	m_rectangle.top = frameRow * m_height;
	m_speed = speed;
	m_currentFrame = startFrame;
	m_type = looping;
	m_timer = 0;
}

void Animation::playOnce(int startFrame, int endFrame, int frameRow, float speed)
{
	m_startFrame = startFrame;
	m_endFrame = endFrame;
	m_rectangle.top = frameRow * m_height;
	m_currentFrame = startFrame;
	m_speed = speed;
	m_type = once;
	m_timer = 0;
}

void Animation::stillFrame(int frame, int row)
{
	m_rectangle.top = row * m_height;
	m_currentFrame = frame;
	m_type = still;
}


sf::IntRect Animation::getRectangle(float deltaTime)
{
	m_timer += deltaTime;

	if (m_type == looping)
	{
		//1/speed = number of seconds per frame.
		if (1/m_speed <= m_timer)
		{
			m_currentFrame++;

			if (m_currentFrame > m_endFrame)
				m_currentFrame = m_startFrame;

			//decrease timer with time taken per frame
			m_timer -= 1/m_speed;
		}
	}

	if (m_type == once)
	{
		if (1 / m_speed <= m_timer)
		{
			m_currentFrame++;

			if (m_currentFrame > m_endFrame)
				stillFrame(m_endFrame, m_rectangle.top);

			m_timer -= 1 / m_speed;
		}
	}

	m_rectangle.left = m_currentFrame * m_width;

	return m_rectangle;
}

void Animation::setSpeed(float speed)
{
	m_speed = speed;

}