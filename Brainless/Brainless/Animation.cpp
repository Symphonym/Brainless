#include "Animation.h"

Animation::Animation(int width, int height)
:
m_width(width),
m_height(height),
rectangle(0, 0, width, height)
{
}

 void Animation::animate(int startFrame, int endFrame, int frameRow, float speed)
{
	 m_startFrame = startFrame;
	 m_endFrame = endFrame;
	 rectangle.top = frameRow * m_height;
	 m_currentFrame = startFrame;
}


 void Animation::update()
 {
	 m_timer -= 1;

	 if (m_timer <= 0)
	 {
		 m_currentFrame++;

		 if (m_currentFrame > m_endFrame)
			 m_currentFrame = m_startFrame;

		 m_timer = m_delay;
	 }

	 rectangle.left = m_currentFrame * m_width;
 }