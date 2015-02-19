#include "RobotAkeAttack.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include "Utility.h"
//#include "ParticleSystem.h"

RobotAkeAttack::RobotAkeAttack(ArcadeMachine &machine)
:
ArcadeGame(machine, "Robot Åke Attack"),
m_gamePos(m_machine.getScreenPos()),
m_score(0)
{
	hitbox.setTexture(ResourceLoader::instance().retrieveTexture("spr_player"));
	hitbox.setColor(sf::Color(0, 0, 0, 128));
	//Set player
	m_player->sprite.setTexture(ResourceLoader::instance().retrieveTexture("spr_player"));
	//Load backgrounds
	m_backgrounds[0].setTexture(ResourceLoader::instance().retrieveTexture("bc_sky"));
	m_backgrounds[0].setPosition(m_gamePos);
	m_backgrounds[0].setScale(2, 2);
}

void RobotAkeAttack::onGameStart()
{
	//Clear old
	for (int i = m_pillars.size()-1; i >= 0 ;i--)
	{
		delete m_pillars[i];
		m_pillars.pop_back();
	}
	m_speed = 300;
	//Player
	m_player->box = sf::FloatRect(0, 0, 64, 64);
	m_player->box.left = m_gamePos.x + 40;
	m_player->sprite.setScale(64, 64);
	m_player->y_speed = 0;
	m_player->jumps = 2;
	pillar* temp = new pillar();
	//Pillars
	temp->sprite.setPosition(m_gamePos + sf::Vector2f(700,400));
	temp->m_width = 300;
	temp->m_boxes.push_back(new sf::FloatRect(temp->sprite.getPosition().x, temp->sprite.getPosition().y, 300, 100));
	m_pillars.push_back(temp);
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
	m_speed += deltaTime;
	//Player
	//Commands
	if (m_player->event_jump)
	{
		m_player->event_jump = false;
		if (m_player->jumps > 0)
		{
			m_player->jumps--;
			m_player->y_speed = -2;
		}
	}
	//Check collision below - if move to contact
	//Physics
	m_player->y_speed = Utility::clampValue<float>(m_player->y_speed + (c_gravity* deltaTime), -200, 200);
	
	//Keep player within bounds
	m_player->box.top = Utility::clampValue<float>(m_player->box.top, m_gamePos.y, m_gamePos.y + 700 - m_player->box.height);
	//Update pillars
	for (int i = 0; i < m_pillars.size(); i++)
	{
		m_pillars[i]->sprite.setPosition(m_pillars[i]->sprite.getPosition() - sf::Vector2f(m_speed*deltaTime, 0));
		for (int j = 0; j < m_pillars[i]->m_boxes.size(); j++)
		{
			if (m_pillars[i]->m_boxes[i]->intersects(m_player->box))
			{
				if (m_player->y_speed>0)
				{
					m_player->box.top = m_pillars[i]->m_boxes[i]->top - m_player->box.height;
					m_player->y_speed = 0;
					m_player->jumps = 2;
				}	
			}
			else if (m_pillars[i]->m_boxes[i]->intersects(m_player->box))
			{
				//Game over
			}
			m_pillars[i]->m_boxes[i]->intersects(m_player->box);
			m_pillars[i]->m_boxes[i]->left -= m_speed*deltaTime;
		}
		//Move back
		if (m_pillars[i]->sprite.getPosition().x + m_pillars[i]->m_width < m_gamePos.x)
		{
			m_pillars[i]->sprite.setPosition(m_pillars[i]->sprite.getPosition() + sf::Vector2f(900, 0));
			for (int j = 0; j < m_pillars[i]->m_boxes.size(); j++)
			{
				m_pillars[i]->m_boxes[i]->left += 900;
				m_pillars[i]->m_boxes[i]->top = m_gamePos.y + 200 + std::rand() % 300;
			}
		}
	}
	m_player->box.top += m_player->y_speed;
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
			hitbox.setScale(m_pillars[i]->m_boxes[i]->width, m_pillars[i]->m_boxes[i]->height);
			hitbox.setPosition(sf::Vector2f(m_pillars[i]->m_boxes[i]->left, m_pillars[i]->m_boxes[i]->top));
			Renderer::instance().drawHUD(hitbox);
		}
	}
	//Draw player
	m_player->sprite.setPosition(sf::Vector2f(m_player->box.left, m_player->box.top));
	Renderer::instance().drawHUD(m_player->sprite);
}