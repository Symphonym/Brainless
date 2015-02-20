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
	//Stats
	int jumps = 2;
	float dash_cd = 0;
	//variables
	float y_speed = 0;
};

//Objects
struct pillar
{
	sf::Vector2f position;
	sf::Sprite sprite;
	int m_width;
	std::vector <sf::FloatRect> m_boxes;
};

RobotAkeAttack::RobotAkeAttack(ArcadeMachine &machine)
:
ArcadeGame(machine,"Robot Åke Attack"),
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
	while (m_pillars.size() > 0)
	{
		delete m_pillars[0];
		m_pillars.erase(m_pillars.begin());
	}
	delete m_player;

	m_timer = 0;
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
	m_pillars.push_back(createPillar(1, m_gamePos + sf::Vector2f(300, 400)));
	//m_pillars.push_back(createPillar(1, m_gamePos + sf::Vector2f(700, 500)));
}

void RobotAkeAttack::events(const sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::D && m_player->dash_cd<=0)
		{
			m_player->dash_cd = 0.8;
			m_player->y_speed = 0;
			if (m_player->jumps == 0)
				m_player->jumps++;
		}
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::S && m_player->jumps > 0)
		{
			m_player->jumps--;
			m_player->y_speed = -600;
			
		}
	}
}

void RobotAkeAttack::update(float deltaTime)
{
	m_timer += deltaTime;
	m_speed = 400 +(m_timer*40);
	//Dash boost
	if (m_player->dash_cd > 0.3)
		m_speed += 200;
	//Player speed
	else
		m_player->y_speed = m_player->y_speed + (c_gravity* deltaTime);//Utility::clampValue<float>(m_player->y_speed + (c_gravity* deltaTime), -20000, 20000);
	//Update pillars
	for (int i = m_pillars.size() - 1; i >= 0; i--)
	{
		m_pillars[i]->position -= sf::Vector2f(m_speed*deltaTime, 0);
		for (int j = 0; j < m_pillars[i]->m_boxes.size(); j++)
		{
			if (m_pillars[i]->m_boxes[j].intersects(m_player->box))
			{
				int hight_diffrence = (m_player->box.top + m_player->box.height) - m_pillars[i]->m_boxes[j].top;
				if (hight_diffrence >= 16)
				{
					m_gameOver_timer = 2;
				}
				else
				{
					m_player->box.top = m_pillars[i]->m_boxes[j].top - m_player->box.height;
					if (m_player->y_speed > 0)
					{
						m_player->y_speed = 0;
						m_player->jumps = 2;
					}
				}
			}
			//m_pillars[i]->m_boxes[j].intersects(m_player->box);
			m_pillars[i]->m_boxes[j].left -= m_speed*deltaTime;
		}
		//Exit map
		if (m_pillars[i]->position.x + m_pillars[i]->m_width < m_gamePos.x)
		{
			delete m_pillars[i];
			m_pillars.erase(m_pillars.begin() + i);
			m_pillars.push_back(createPillar(1 + rand()%2, m_gamePos + sf::Vector2f(700, 200 + std::rand() % 300)));
		}
	}
	//Update stars
	for (int i = m_stars.size() - 1; i >= 0; i--)
	{
		m_stars[i]->setPosition(m_stars[i]->getPosition() - sf::Vector2f(m_speed*deltaTime, 0));
		if (m_stars[i]->getPosition().x + 64 < m_gamePos.x)
		{
			delete m_stars[i];
			m_stars.erase(m_stars.begin() + i);
		}
	}
	//Player gravity
	if (m_player->dash_cd <= 0.3)
	{
		if (m_player->y_speed < 0)
			m_player->y_speed += 300 * deltaTime;
		m_player->box.top += m_player->y_speed*deltaTime;
	}
	//Keep player within bounds
	m_player->box.top = std::max(m_player->box.top, m_gamePos.y);
	if (m_player->box.top >= m_gamePos.y + 700 - m_player->box.height)
	{
		m_gameOver_timer = 2;
	}
	//Decrement dash cd
	m_player->dash_cd -= deltaTime;
	if (m_player->dash_cd < 0)
		m_player->dash_cd = 0;
	//End game
	if (m_gameOver_timer>0)
	{
		onGameStart();
		m_gameOver_timer = 0;
	}
}

void RobotAkeAttack::draw()
{
	//Draw backgrounds
	Renderer::instance().drawHUD(m_backgrounds[0]);
	//Draw pillars
	for (int i = 0; i < m_pillars.size(); i++)
	{
		int start_offset = std::max(0, (int)(m_gamePos.x - m_pillars[i]->position.x));
		int end_offset = std::min(0, (int)((m_gamePos.x+700) - (m_pillars[i]->position.x + m_pillars[i]->m_width)));
		m_pillars[i]->sprite.setTextureRect(sf::IntRect(start_offset*0.5,0,270-start_offset*0.5+end_offset*0.5,57));
		m_pillars[i]->sprite.setPosition(m_pillars[i]->position + sf::Vector2f(start_offset,0));
		Renderer::instance().drawHUD(m_pillars[i]->sprite);
		for (int j = 0; j < m_pillars[i]->m_boxes.size(); j++)
		{
			hitbox.setScale(m_pillars[i]->m_boxes[j].width, m_pillars[i]->m_boxes[j].height);
			hitbox.setPosition(sf::Vector2f(m_pillars[i]->m_boxes[j].left, m_pillars[i]->m_boxes[j].top));
			Renderer::instance().drawHUD(hitbox);
		}
	}
	//Draw player
	m_player->sprite.setPosition(sf::Vector2f(m_player->box.left, m_player->box.top));
	Renderer::instance().drawHUD(m_player->sprite);
}

pillar* RobotAkeAttack::createPillar(int type, sf::Vector2f position)
{
	pillar* temp = new pillar();
	temp->position = position;
	temp->sprite.setTexture(ResourceLoader::instance().retrieveTexture("pillar1"));
	//temp->sprite.setTexture(ResourceLoader::instance().retrieveTexture("pillar" + std::to_string(type)));
	temp->sprite.setScale(2,2);
	temp->m_width = temp->sprite.getTexture()->getSize().x*2;
	temp->sprite.setPosition(position);
	switch (type)
	{
	case 1:
		temp->m_boxes.push_back(sf::FloatRect(temp->sprite.getPosition().x, temp->sprite.getPosition().y,540, 80));
		break;
	case 2:
		temp->m_boxes.push_back(sf::FloatRect(temp->sprite.getPosition().x, temp->sprite.getPosition().y, 540, 80));
		temp->m_boxes.push_back(sf::FloatRect(temp->sprite.getPosition().x+260, temp->sprite.getPosition().y-30, 200, 30));
		break;
	}
	return temp;
}