#ifndef INCLUDED_ANIMATION_H
#define INCLUDED_ANIMATION_H

#include <SFML\Graphics.hpp>

class Animation
{
public:
	Animation(int width, int height, float speed, sf::Texture &spriteSheet);

	void animate(int startFrame, int endFrame, int frameRow);
	void update();

private:
	sf::IntRect m_rectangle;
	float m_speed;
};

#endif