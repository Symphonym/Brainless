#ifndef INCLUDED_ANIMATION_H
#define INCLUDED_ANIMATION_H

#include <SFML\Graphics.hpp>



class Animation
{
public:
	Animation(int width, int height);

	enum AnimationType
	{
		looping = 0,
		once = 1,
		still = 2
	};

	//Plays the selected frames over and over again
	void loop(int startFrame, int endFrame, int frameRow, float speed);

	//Plays the selected frames once and stops at the last frame
	void playOnce(int startFrame, int endFrame, int frameRow, float speed);

	//Shows only the selected frame
	void stillFrame(int Frame, int Row);

	//Updates the rectangles position
	sf::IntRect getRectangle(float deltaTime);
	float getWidth(){ return m_width; }
	float getHeight(){ return m_height; }
private:
	int m_height;
	int m_width;
	float m_speed;
	int m_startFrame;
	int m_endFrame;
	int m_currentFrame;
	sf::IntRect m_rectangle;
	AnimationType m_type;
	
	float m_timer;
	float m_delay = 100;
};

#endif