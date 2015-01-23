#ifndef INCLUDED_ANIMATION_H
#define INCLUDED_ANIMATION_H

#include <SFML\Graphics.hpp>

class Animation
{
public:
	Animation(int width, int height);

	//Sets which frames will be animated
	void animate(int startFrame, int endFrame, int frameRow, float speed);

	//Updates the rectangles position
	void update();

	sf::IntRect rectangle;

private:
	int m_height;
	int m_width;
	float m_speed;
	int m_startFrame;
	int m_endFrame;
	int m_currentFrame;
	sf::Texture m_texture;
	
	float m_timer;
	float m_delay = 100;
};

#endif