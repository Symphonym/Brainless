#ifndef INCLUDED_TURTLE_H
#define INCLUDED_TURTLE_H

#include <SFML\Graphics.hpp>

#include "ArcadeGame.h"
#include "Animation.h"
enum stuffType
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
	stuff(stuffType _type, int _x, int _y);

	stuffType type;
	int x;
	int y;
	int oldX;
	int oldY;
	stuff* next;
	stuff* before;
	sf::Sprite sprite;
	Animation animation;
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

	sf::Sprite background;

	sf::Vector2f screenPos;

	stuff* map[10][10];
	int score;
	sf::Text scoreText;
	sf::Text infoText;
	bool infoShowing = true;

	bool isDead = false;
};

#endif