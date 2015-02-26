#ifndef INCLUDED_TURTLE_H
#define INCLUDED_TURTLE_H

#include <SFML\Graphics.hpp>

#include "ArcadeGame.h"
#include "Animation.h"
enum pointType
{
	turtle,
	frie
};

enum dirr
{
	up,
	down,
	left,
	right
};
class stuff
{
public:
	stuff(pointType _type, int _x, int _y):type(_type),x(_x),y(_y),oldX(_x),oldY(_y){};
	pointType type;
	int x;
	int y;
	int oldX;
	int oldY;
	stuff* next;
	stuff* before;
};

class Turtle : public ArcadeGame
{
public:

	Turtle(ArcadeMachine &machine);
	virtual void onGameStart();
	~Turtle();
	virtual void update(float deltaTime);
	virtual void draw();

private:
	float time;
	dirr direction;
	stuff* head;
	sf::Sprite m_turtleSprite;
	Animation m_turtleAnimation;

	sf::Sprite m_frieSprite;
	Animation m_frieAnimation;

	sf::Sprite m_background;

	sf::Vector2f m_screenPos;

	stuff* map[10][10];
	int m_score;
	sf::Text m_scoreText;
	sf::Text m_infoText;
	bool m_infoShowing = true;

	bool isDead = false;
};

#endif