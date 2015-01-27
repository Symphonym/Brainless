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
}

void Animation::playOnce(int startFrame, int endFrame, int frameRow, float speed)
{
	m_startFrame = startFrame;
	m_endFrame = endFrame;
	m_rectangle.top = frameRow * m_height;
	m_currentFrame = startFrame;
	m_speed = speed;
	m_type = once;
}

void Animation::stillFrame(int frame, int row)
{
	m_rectangle.top = row * m_height;
	m_currentFrame = frame;
	m_type = still;
}


sf::IntRect Animation::getRectangle(float deltaTime)
{
	m_timer -= deltaTime * m_speed;

	if (m_type == looping)
	{
		if (m_timer <= 0)
		{
			m_currentFrame++;

			if (m_currentFrame > m_endFrame)
				m_currentFrame = m_startFrame;

			m_timer = m_delay;
		}
	}

	if (m_type == once)
	{
		if (m_timer <= 0)
		{
			m_currentFrame++;

			if (m_currentFrame > m_endFrame)
				stillFrame(m_endFrame, m_rectangle.top);

			m_timer = m_delay;
		}
	}

	m_rectangle.left = m_currentFrame * m_width;

	return m_rectangle;
}