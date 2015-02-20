#include "RobotAkeAttack.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include <math.h>
#include "Utility.h"
//#include "ParticleSystem.h"

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

//Objects
struct pillar
{
	sf::Sprite sprite;
	int m_width;
	std::vector <sf::FloatRect> m_boxes;
};

RobotAkeAttack::RobotAkeAttack(ArcadeMachine &machine)
:
ArcadeGame(machine, "Robot Åke Attack"),
m_gamePos(m_machine.getScreenPos()),
m_score(0)
{
	hitbox.setTexture(ResourceLoader::instance().retrieveTexture("spr_player"));
	hitbox.setColor(sf::Color(0, 0, 0, 128));
	//Load backgrounds
	m_backgrounds[0].setTexture(ResourceLoader::instance().retrieveTexture("bc_sky"));
	m_backgrounds[0].setPosition(m_gamePos);
	m_backgrounds[0].setScale(2, 2);
}

void RobotAkeAttack::onGameStart()
{
	//Clean old
	while (m_pillars.size()>0)
	{
		delete m_pillars[0];
		m_pillars.erase(m_pillars.begin());
	}
	delete m_player;

	m_speed = 300;
	//Player
	m_player = new tort();
	m_player->box = sf::FloatRect(0, 0, 64, 64);
	m_player->box.left = m_gamePos.x + 40;
	m_player->sprite.setScale(64, 64);
	m_player->sprite.setTexture(ResourceLoader::instance().retrieveTexture("spr_player"));
	//m_player->y_speed = 0;
	//m_player->jumps = 2;
	pillar* temp = new pillar();
	//Pillars
	m_pillars.push_back(createPillar(1, m_gamePos + sf::Vector2f(700, 400)));
	m_pillars.push_back(createPillar(1, m_gamePos + sf::Vector2f(300, 500)));
}

void RobotAkeAttack::events(const sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::A)
			m_player->event_jump = true;
		else if (event.key.code == sf::Keyboard::S)
			m_player->event_dash = true;
	}
}

void RobotAkeAttack::update(float deltaTime)
{
	m_speed += deltaTime*20;
	//Player
	//Commands
	if (m_player->event_jump)
	{
		m_player->event_jump = false;
		if (m_player->jumps > 0)
		{
			m_player->jumps--;
			m_player->y_speed = -600;
		}
	}
	//Check collision below - if move to contact
	//Physics
	m_player->y_speed = Utility::clampValue<float>(m_player->y_speed + (c_gravity* deltaTime), -2000, 2000);
	
	//Update pillars
	for (int i = m_pillars.size()-1; i >=0; i--)
	{
		m_pillars[i]->sprite.setPosition(m_pillars[i]->sprite.getPosition() - sf::Vector2f(m_speed*deltaTime, 0));
		for (int j = 0; j < m_pillars[i]->m_boxes.size(); j++)
		{
			if (m_pillars[i]->m_boxes[j].intersects(m_player->box))
			{
				if (m_player->y_speed>0)
				{
					m_player->box.top = m_pillars[i]->m_boxes[j].top - m_player->box.height;
					m_player->y_speed = 0;
					m_player->jumps = 2;
				}	
			}
			else if (m_pillars[i]->m_boxes[j].intersects(m_player->box))
			{
				//Game over
			}
			m_pillars[i]->m_boxes[j].intersects(m_player->box);
			m_pillars[i]->m_boxes[j].left -= m_speed*deltaTime;
		}
		//Exit map
		if (m_pillars[i]->sprite.getPosition().x + m_pillars[i]->m_width < m_gamePos.x)
		{
			delete m_pillars[i];
			m_pillars.erase(m_pillars.begin() + i);
			m_pillars.push_back(createPillar(1, m_gamePos + sf::Vector2f(700, 200 + std::rand() % 300)));
		}
	}
	m_player->box.top += m_player->y_speed*deltaTime;
	//Keep player within bounds
	m_player->box.top = std::max(m_player->box.top, m_gamePos.y);
	if (m_player->box.top >= m_gamePos.y + 700 - m_player->box.height)
	{
		onGameStart();
	}
}

void RobotAkeAttack::draw()
{
	//Draw backgrounds
	Renderer::instance().drawHUD(m_backgrounds[0]);
	//Draw pillars
	for (int i = 0; i < m_pillars.size(); i++)
	{
		Renderer::instance().drawHUD(m_pillars[i]->sprite);
		for (int j = 0; j < m_pillars[i]->m_boxes.size(); j++)
		{
			hitbox.setScale(m_pillars[i]->m_boxes[j].width, m_pillars[j]->m_boxes[j].height);
			hitbox.setPosition(sf::Vector2f(m_pillars[i]->m_boxes[j].left, m_pillars[i]->m_boxes[j].top));
			Renderer::instance().drawHUD(hitbox);
		}
	}
	//Draw player
	m_player->sprite.setPosition(sf::Vector2f(m_player->box.left, m_player->box.top));
	Renderer::instance().drawHUD(m_player->sprite);
}

pillar* RobotAkeAttack::createPillar(int type,sf::Vector2f position)
{
	pillar* temp = new pillar();
	temp->sprite.setTexture(ResourceLoader::instance().retrieveTexture("pillar" + std::to_string(type)));
	temp->sprite.setPosition(position);
	switch (type)
	{
	case 1:
		temp->m_width = 270;
		temp->m_boxes.push_back(sf::FloatRect(temp->sprite.getPosition().x, temp->sprite.getPosition().y, 270, 48));
		break;
	}
	return temp;
}