#ifndef INCLUDED_ROBOT_AKE_ATTACK_H
#define INCLUDED_ROBOT_AKE_ATTACK_H

#include <vector>

#include "ArcadeGame.h"
#include "ArcadeMachine.h"
#include <SFML\Graphics.hpp>

struct tort;
struct pillar;
struct star;

class RobotAkeAttack : public ArcadeGame
{
public:

	RobotAkeAttack(ArcadeMachine &machine);

	virtual void onGameStart();

	virtual void events(const sf::Event &event);
	virtual void update(float deltaTime);
	virtual void draw();

private:
	
	//Variables
	int m_score;
	float m_timer = 0;
	float m_gameOver_timer = 0;
	float m_speed;
	const float c_gravity = 900;
	sf::Vector2f m_gamePos;
	sf::Sprite hitbox;

	//player
	tort* m_player;

	//Objects
	std::vector<pillar*> m_pillars;
	std::vector<sf::Sprite*> m_stars;

	//Visuall
	sf::Sprite m_backgrounds[1];
	//Functions
	pillar* createPillar(int type, sf::Vector2f position);
};

#endif