#ifndef INCLUDED_ROBOT_AKE_ATTACK_H
#define INCLUDED_ROBOT_AKE_ATTACK_H

#include <vector>

#include "ArcadeGame.h"
#include "ArcadeMachine.h"
#include <SFML\Graphics.hpp>

struct tort;
struct pillar;

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
	sf::Vector2f m_gamePos;
	sf::Text m_scoreText;
	float m_score, m_hscore;

	float m_timer = 0;
	float m_pillarTimer;
	bool m_gameOver = false;
	float m_speed;

	const float c_gravity = 900;
	
	//player
	tort* m_player;
	sf::Sprite m_dash;

	//Objects
	std::vector<pillar*> m_pillars;
	std::vector<sf::Sprite*> m_stars;
	std::vector<sf::Text> m_texts;
	const int c_starSize = 96;

	//Visuall
	sf::Sprite m_backgrounds[8];
	//Functions
	pillar* createPillar(int type, sf::Vector2f position,bool star);
	void createStar(sf::Vector2f position);
};

#endif