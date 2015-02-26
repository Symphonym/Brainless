#include "Animation.h"

#define ENDFRAME 8 //spritesheet storlek

Animation::Animation(int width, int height)
:
m_width(width),
m_height(height),
m_rectangle(0, 0, width, height),
m_playOnceDone(false),
m_reverse(false)
{
}

Animation::Animation()
:
m_width(256),
m_height(256),
m_rectangle(0, 0, 256, 256),
m_playOnceDone(false),
m_reverse(false)
{
}


void Animation::loop(int startFrame, int endFrame, int frameRow, float speed)
{
	m_startFrame = startFrame;
	m_endFrame = endFrame;
	m_speed = speed;
	m_currentFrame = startFrame;
	m_type = looping;
	m_timer = 0;
	m_startRow = frameRow;
	m_endRow = frameRow;
	m_currentRow = frameRow;
	m_playOnceDone = false;
	m_reverse = false;
}

void Animation::loop(int startFrame, int endFrame, int startRow, int endRow, float speed)
{
	m_startFrame = startFrame;
	m_endFrame = endFrame;
	m_speed = speed;
	m_currentFrame = startFrame;
	m_type = looping;
	m_timer = 0;
	m_startRow = startRow;
	m_endRow = endRow;
	m_currentRow = startRow;
	m_playOnceDone = false;
	m_reverse = false;
}
void Animation::playOnce(int startFrame, int endFrame, int frameRow, float speed)
{
	m_startFrame = startFrame;
	m_endFrame = endFrame;
	m_currentFrame = startFrame;
	m_speed = speed;
	m_type = once;
	m_timer = 0;
	m_startRow = frameRow;
	m_endRow = frameRow;
	m_currentRow = frameRow;
	m_playOnceDone = false;
	m_reverse = false;
}
void Animation::playOnce(int startFrame, int endFrame, int startRow, int endRow, float speed)
{
	m_startFrame = startFrame;
	m_endFrame = endFrame;
	m_currentFrame = startFrame;
	m_speed = speed;
	m_type = once;
	m_timer = 0;
	m_startRow = startRow;
	m_endRow = endRow;
	m_currentRow = startRow;
	m_playOnceDone = false;
	m_reverse = false;
}

void Animation::stillFrame(int frame, int row)
{
	m_currentRow = row;
	m_currentFrame = frame;
	m_type = still;
}


sf::IntRect Animation::getRectangle(float deltaTime)
{
	m_timer += deltaTime;

	if (m_type == looping)
	{
		//1/speed = number of seconds per frame.
		if (1 / m_speed <= m_timer)
		{
			//play animation forward
			if (!m_reverse)
			{
				m_currentFrame++;

				//restart loop
				if (m_currentRow == m_endRow && m_currentFrame > m_endFrame)
				{
					m_playOnceDone = true;
					m_currentFrame = m_startFrame;
					m_currentRow = m_startRow;
				}
				//next row
				else if (m_currentFrame == ENDFRAME)
				{
					m_currentRow += 1;
					m_currentFrame = 0;
				}

			}
			//play backward
			else
			{
				m_currentFrame--;

				//restart loop
				if (m_currentRow == m_startRow && m_currentFrame < m_startFrame)
				{
					m_playOnceDone = true;
					m_currentFrame = m_endFrame;
					m_currentRow = m_endRow;
				}
				//next row
				else if (m_currentFrame == -1)
				{
					m_currentRow -= 1;
					m_currentFrame = ENDFRAME-1;
				}

			}
			//decrease timer with time taken per frame
			m_timer -= 1 / m_speed;
		}
	}

	if (m_type == once)
	{
		if (1 / m_speed <= m_timer)
		{
			//play animation forward
			if (!m_reverse)
			{
				m_currentFrame++;

				//stop loop at endFrame
				if (m_currentRow == m_endRow && m_currentFrame > m_endFrame)
				{
					m_playOnceDone = true;
					stillFrame(m_endFrame, m_endRow);
				}
				//next row
				else if (m_currentFrame == ENDFRAME)
				{
					m_currentRow += 1;
					m_currentFrame = 0;
				}
			}
			else
			{
				//EJ IMPLEMENTERAT

				//m_currentFrame++;

				////stop loop at endFrame
				//if (m_currentRow == m_endRow && m_currentFrame > m_endFrame)
				//{
				//	m_playOnceDone = true;
				//	stillFrame(m_endFrame, m_endRow);
				//}
				////next row
				//else if (m_currentFrame == ENDFRAME)
				//{
				//	m_currentRow += 1;
				//	m_currentFrame = 0;
				//}
			}
			

			m_timer -= 1 / m_speed;
		}
	}

	m_rectangle.top = m_currentRow * m_height;
	m_rectangle.left = m_currentFrame * m_width;

	return m_rectangle;
}

void Animation::setSpeed(float speed)
{
	m_speed = speed;
}

void Animation::setReverse(bool reverse)
{
	m_reverse = reverse;
}

void Animation::setHeight(int height)
{
	m_height = height;
	m_rectangle.height = height;
}

void Animation::setWidth(int width)
{
	m_width = width;
	m_rectangle.width = width;
}

float Animation::getWidth(){ return m_width; }
float Animation::getHeight(){ return m_height; }
int Animation::getCurrentFrame(){ return m_currentFrame; }
bool Animation::getPlayOnceDone(){ return m_playOnceDone; }
/*
returns speed closer minSpeed when value is closer to useMinValue
returns speed closer maxSpeed when value is closer to useMaxValue
useMinValue can be higher than useMaxValue
*/
float Animation::calcFrameSpeed(float minSpeed, float maxSpeed, float useMinValue, float useMaxValue, float value)
{
	if (minSpeed == maxSpeed) return minSpeed;

	//"Reverted"
	if (useMaxValue < useMinValue)
	{
		if (value < useMaxValue) return maxSpeed;
		else if (useMinValue < value) return minSpeed;
		float b = useMinValue - useMaxValue;
		float a = abs(value) - useMaxValue;
		if (a == 0) return maxSpeed;
		return maxSpeed - (maxSpeed - minSpeed)*(b / a);
	}
	//Normal
	if (useMaxValue < value) return maxSpeed;
	else if (value < useMinValue) return minSpeed;
	float a = useMaxValue - useMinValue;
	float b = abs(value) - useMinValue;
	if (a == 0) return minSpeed;
	return minSpeed + (maxSpeed - minSpeed)*(b / a);
}
