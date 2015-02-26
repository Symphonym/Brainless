#include "RobotAkeAttack.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include <math.h>
#include "Utility.h"
#include "OptionsMenu.h"
//#include "ParticleSystem.h"

//Player
struct tort
{
	sf::Sprite sprite;
	sf::FloatRect box;
	//Stats
	int jumps = 2;
	int dashs = 2;
	float dash_cd = 0;
	int star_mult = 0;

	bool jump_key = false;
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
ArcadeGame(machine, "Robot Åke Attack"),
m_gamePos(m_machine.getScreenPos()),
m_score(0)
{
	//Load font
	m_scoreText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_scoreText.setString("");

	m_dash.setTexture(ResourceLoader::instance().retrieveTexture("spr_dash"));
	//Load backgrounds
	m_backgrounds[0].setTexture(ResourceLoader::instance().retrieveTexture("bc_sky"));
	m_backgrounds[0].setPosition(m_gamePos);
	m_backgrounds[0].setScale(2, 2);
	m_backgrounds[7].setTexture(ResourceLoader::instance().retrieveTexture("bc_frontClouds"));
	m_backgrounds[7].setScale(2, 2);
	m_backgrounds[7].setPosition(m_gamePos + sf::Vector2f(0, 286));
}

void RobotAkeAttack::onGameStart()
{
	//Clean old
	while (m_stars.size() > 0)
	{
		delete m_stars[0];
		m_stars.erase(m_stars.begin());
	}
	while (m_pillars.size() > 0)
	{
		delete m_pillars[0];
		m_pillars.erase(m_pillars.begin());
	}
	delete m_player;
	m_score = 0;
	m_timer = 0;
	m_pillarTimer = 3;
	m_gameOver = false;
	//Player
	m_player = new tort();
	m_player->box = sf::FloatRect(8, 4, 56, 32);
	m_player->box.left = m_gamePos.x + 60;
	m_player->box.top = m_gamePos.y + 100;
	m_player->sprite.setScale(2, 2);
	m_player->sprite.setTexture(ResourceLoader::instance().retrieveTexture("spr_tort"));
	pillar* temp = new pillar();
	//Pillars
	m_pillars.push_back(createPillar(1, m_gamePos + sf::Vector2f(100, 250), false));
	m_pillars.push_back(createPillar(3, m_gamePos + sf::Vector2f(600, 400), false));
}

void RobotAkeAttack::events(const sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == OptionsMenu::getKeybind("Right") && m_player->dash_cd <= 0 && m_player->dashs > 0)
		{
			m_player->dash_cd = 0.8;
			m_player->dashs--;
			m_player->y_speed = 0;
			if (m_player->jumps == 0)
				m_player->jumps++;
		}
		else if (event.key.code == OptionsMenu::getKeybind("Down") && m_player->jumps > 0 && !m_player->jump_key)
		{
			m_player->jump_key = true;
			m_player->jumps--;
			m_player->y_speed = -850; //500
		}
		else if (event.key.code == OptionsMenu::getKeybind("Up") && m_gameOver)
		{
			onGameStart();
		}
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == OptionsMenu::getKeybind("Down"))
		{
			m_player->jump_key = false;
		}
	}
}

void RobotAkeAttack::update(float deltaTime)
{
	if (!m_gameOver)
	{
		m_timer += deltaTime;
		m_score += 100 * deltaTime;
		m_speed = 300 + (m_timer * 10);
		//Dash boost
		if (m_player->dash_cd > 0.3)
			m_speed += 200;
		//Player speed
		else
		{
			m_player->y_speed = m_player->y_speed + (c_gravity* deltaTime);//Utility::clampValue<float>(m_player->y_speed + (c_gravity* deltaTime), -20000, 20000);
			if (m_player->y_speed < 0)
				m_player->y_speed -= (m_player->y_speed * 3 * deltaTime);
		}
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
						m_gameOver = true;
					}
					else
					{
						m_player->box.top = m_pillars[i]->m_boxes[j].top - m_player->box.height;
						if (m_player->y_speed > 0)
						{
							m_player->y_speed = 0;
							m_player->jumps = 2;
							m_player->dashs = 2;
						}
					}
				}
				m_pillars[i]->m_boxes[j].left -= m_speed*deltaTime;
			}
			//Exit map
			if (m_pillars[i]->position.x + m_pillars[i]->m_width < m_gamePos.x)
			{

				delete m_pillars[i];
				m_pillars.erase(m_pillars.begin() + i);
			}
		}
		//Update stars
		for (int i = m_stars.size() - 1; i >= 0; i--)
		{
			m_stars[i]->setPosition(m_stars[i]->getPosition() - sf::Vector2f(m_speed*deltaTime, 0));
			if (sf::FloatRect(m_stars[i]->getPosition().x, m_stars[i]->getPosition().y, c_starSize, c_starSize).intersects(m_player->box))
			{
				if (m_player->dash_cd > 0)
				{
					m_player->star_mult++;
					m_score += m_player->star_mult * 100;
					m_texts.push_back(sf::Text(std::to_string(m_player->star_mult) + "x100", ResourceLoader::instance().retrieveFont("DefaultFont")));
					m_texts[m_texts.size() - 1].setPosition(m_stars[i]->getPosition());
					delete m_stars[i];
					m_stars.erase(m_stars.begin() + i);
				}
				else
				{
					m_gameOver = true;
				}
			}
			else if (m_stars[i]->getPosition().x + c_starSize < m_gamePos.x)
			{
				m_player->star_mult = 0;
				delete m_stars[i];
				m_stars.erase(m_stars.begin() + i);
			}

		}
		//Player gravity
		if (m_player->dash_cd <= 0.3)
		{
			m_player->box.top += m_player->y_speed*deltaTime;
		}
		//Keep player within bounds
		m_player->box.top = std::max(m_player->box.top, m_gamePos.y);
		if (m_player->box.top >= m_gamePos.y + 700 - m_player->box.height)
		{
			m_gameOver = true;
		}
		//Decrement dash cd
		m_player->dash_cd -= deltaTime;
		if (m_player->dash_cd < 0)
			m_player->dash_cd = 0;
		//Decrement Pillar timer
		m_pillarTimer -= deltaTime;
		if (m_pillarTimer <= 0)
		{
			m_pillars.push_back(createPillar(1 + rand() % 3, m_gamePos + sf::Vector2f(700, Utility::clampValue<float>(m_player->sprite.getPosition().x - 70 + rand() % 140, 200, 500)), rand() % 2));
			m_pillarTimer += 2.5;
			//Adjust for game speed dialtion
			m_pillarTimer = (m_pillarTimer / sqrt(sqrt(sqrt(sqrt(m_timer)))));
		}
		m_scoreText.setPosition(m_gamePos.x, m_gamePos.y + 40);
		m_scoreText.setCharacterSize(32);
		m_scoreText.setString("Score: " + std::to_string((int)floor(m_score)) + "\nHigh score:" + std::to_string((int)floor(m_hscore)));
	}
	else
	{
		m_scoreText.setPosition(m_gamePos.x + 100, m_gamePos.y + 20);
		m_scoreText.setCharacterSize(48);
		m_scoreText.setString("     Game over\n    Score: " + std::to_string((int)floor(m_score)) + "\n   High score:" + std::to_string((int)floor(m_hscore)) + "\n\nPress (W) to restart");
	}
	if (m_score > m_hscore)
	{
		m_hscore = m_score;
	}
	//Update texts
	for (int i = m_texts.size() - 1; i >= 0; i--)
	{
		m_texts[i].setPosition(m_texts[i].getPosition() - sf::Vector2f(0, 100 * deltaTime));
		if (m_texts[i].getPosition().y + 30 < m_gamePos.y)
		{
			m_texts.erase(m_texts.begin() + i);
		}
	}

}

void RobotAkeAttack::draw()
{
	//Draw backgrounds
	Renderer::instance().drawHUD(m_backgrounds[0]);
	m_backgrounds[7].setTextureRect(sf::IntRect((int)(pow(m_timer, 1.3)) % 350, 0, 350, 137));
	Renderer::instance().drawHUD(m_backgrounds[7]);

	//Draw pillars
	for (int i = 0; i < m_pillars.size(); i++)
	{
		int start_offset = std::max(0, (int)(m_gamePos.x - m_pillars[i]->position.x));
		int end_offset = std::min(0, (int)((m_gamePos.x + 700) - (m_pillars[i]->position.x + m_pillars[i]->m_width)));
		m_pillars[i]->sprite.setTextureRect(sf::IntRect(start_offset*0.5, 0, m_pillars[i]->m_width*0.5 - start_offset*0.5 + end_offset*0.5, m_pillars[i]->sprite.getTexture()->getSize().y));
		m_pillars[i]->sprite.setPosition(m_pillars[i]->position + sf::Vector2f(start_offset, 0));
		Renderer::instance().drawHUD(m_pillars[i]->sprite);
		/*for (int j = 0; j < m_pillars[i]->m_boxes.size(); j++)
		{
		sf::Sprite* temp = new sf::Sprite;
		temp->setScale(m_pillars[i]->m_boxes[j].width, m_pillars[i]->m_boxes[j].height);
		temp->setPosition(sf::Vector2f(m_pillars[i]->m_boxes[j].left, m_pillars[i]->m_boxes[j].top));
		temp->setTexture(ResourceLoader::instance().retrieveTexture("spr_player"));
		temp->setColor(sf::Color(0, 0, 0, 128));
		Renderer::instance().drawHUD(*temp);
		}*/
	}
	//Draw stars
	for (int i = 0; i < m_stars.size(); i++)
	{
		if (m_stars[i]->getPosition().x < m_gamePos.x + 700)
			Renderer::instance().drawHUD(*m_stars[i]);
	}
	//Draw player
	if (!m_gameOver)
	{
		m_player->sprite.setPosition(sf::Vector2f(m_player->box.left, m_player->box.top));
		if (m_player->dash_cd > 0)
		{
			m_player->sprite.setTextureRect(sf::IntRect(64, 20, 32, 20));
		}
		else if (abs(m_player->y_speed) < 60)
			m_player->sprite.setTextureRect(sf::IntRect(((int)floor(m_timer*sqrt(m_speed)) % 4) * 32, 0, 32, 20));
		else if (m_player->y_speed > 0)
			m_player->sprite.setTextureRect(sf::IntRect(0, 20, 32, 20));
		else
			m_player->sprite.setTextureRect(sf::IntRect(32, 20, 32, 20));
		Renderer::instance().drawHUD(m_player->sprite);
		if (m_player->dash_cd > 0)
		{
			m_dash.setPosition(m_player->sprite.getPosition() + sf::Vector2f(-65, -40));
			m_dash.setTextureRect(sf::IntRect(round(8 - m_player->dash_cd * 10) * 103, 0, 103, 106));
			Renderer::instance().drawHUD(m_dash);
		}
	}
	Renderer::instance().drawHUD(m_scoreText);
	//Draw Gui
	for (int i = 0; i < m_texts.size(); i++)
	{
		Renderer::instance().drawHUD(m_texts[i]);
	}
}

pillar* RobotAkeAttack::createPillar(int type, sf::Vector2f position, bool star)
{
	pillar* temp = new pillar();
	temp->position = position;
	temp->sprite.setTexture(ResourceLoader::instance().retrieveTexture("pillar" + std::to_string(type)));
	temp->sprite.setScale(2, 2);
	temp->m_width = temp->sprite.getTexture()->getSize().x * 2;
	temp->sprite.setPosition(position);
	switch (type)
	{
	case 1:
		temp->m_boxes.push_back(sf::FloatRect(temp->sprite.getPosition().x, temp->sprite.getPosition().y + 4, 540, 80));
		break;
	case 2:
		temp->m_boxes.push_back(sf::FloatRect(temp->sprite.getPosition().x + 2, temp->sprite.getPosition().y + 75, 863, 80));
		temp->m_boxes.push_back(sf::FloatRect(temp->sprite.getPosition().x + 472, temp->sprite.getPosition().y + 10, 393, 65));
		if (star)
		{
			createStar(position + sf::Vector2f(472, 10 - c_starSize));
		}
		m_pillarTimer += 0.5;
		break;
	case 3:
		temp->m_boxes.push_back(sf::FloatRect(temp->sprite.getPosition().x + 4, temp->sprite.getPosition().y + 12, 1040, 80));
		if (star)
		{
			createStar(position + sf::Vector2f(60 + rand() % 980 - c_starSize, 12 - c_starSize));
		}
		m_pillarTimer += 1;
		break;
	}
	return temp;
}

void RobotAkeAttack::createStar(sf::Vector2f position)
{
	sf::Sprite* temp = new sf::Sprite(ResourceLoader::instance().retrieveTexture("spr_star"));
	temp->setScale(2, 2);
	temp->setPosition(position);
	m_stars.push_back(temp);
}