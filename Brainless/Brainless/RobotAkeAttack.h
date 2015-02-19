#ifndef INCLUDED_ROBOT_AKE_ATTACK_H
#define INCLUDED_ROBOT_AKE_ATTACK_H

#include <vector>

#include "ArcadeGame.h"
#include "ArcadeMachine.h"
#include <SFML\Graphics.hpp>

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
	float m_speed;
	const float c_gravity = 4;
	sf::Vector2f m_gamePos;
	sf::Sprite hitbox;
	//Player
	struct tort
	{
		sf::Sprite sprite;
		sf::FloatRect box;
		//events
		bool event_jump = false;
		bool event_dash = false;
		//Stats
		int jumps = 2;
		bool dash = true;
		//variables
		float y_speed = 0;
	};
	tort* m_player = new tort();
	//Objects
	struct pillar
	{
		sf::Sprite sprite;
		int m_width;
		std::vector <sf::FloatRect*> m_boxes;
	};
	std::vector<pillar*> m_pillars;
	//Visuall
	sf::Sprite m_backgrounds[1];
	
};

#endif