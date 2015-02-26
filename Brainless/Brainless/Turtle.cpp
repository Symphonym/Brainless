#include <iostream>
#include "Turtle.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include "Utility.h"
#include "SoundPlayer.h"
#include "OptionsMenu.h"
//plz no hate on uggly code, much lazy, very kappa
Turtle::Turtle(ArcadeMachine &machine)
:
ArcadeGame(machine, "Turtle"),
m_turtleAnimation(50, 50),
m_frieAnimation(40, 40),
m_screenPos(m_machine.getScreenPos()),
m_score(0)
{
	m_screenPos += sf::Vector2f(20, 20);
	m_turtleSprite.setTexture(ResourceLoader::instance().retrieveTexture("turtlesheet"));
	m_frieSprite.setTexture(ResourceLoader::instance().retrieveTexture("friesheet"));
	m_background.setTexture(ResourceLoader::instance().retrieveTexture("TurtleBackground"));

	m_turtleAnimation.loop(0, 1, 0, 1.5f);
	m_frieAnimation.loop(0, 1, 0, 1.5f);

	m_turtleSprite.setPosition(m_screenPos);

	m_frieSprite.setOrigin(sf::Vector2f(20, 20));

	m_background.setPosition(m_machine.getScreenPos());

	m_scoreText = sf::Text("Score: " + m_score, ResourceLoader::instance().retrieveFont("DefaultFont"));

	m_infoText = sf::Text("Collect the Curly Fries", ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_infoText.setPosition(m_screenPos.x + 150, m_screenPos.y + 335);
	m_infoText.setColor(sf::Color(0, 120, 0, 255));

	m_scoreText.setColor(sf::Color(0, 120, 0, 255));

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			map[i][j] = nullptr;
		}
	}
}

Turtle::~Turtle()
{
	std::cout << "körsinte" << std::endl;
}

void Turtle::onGameStart()
{
	m_turtleSprite.setPosition(m_screenPos.x+300, m_screenPos.y+300);
	m_frieSprite.setPosition(m_screenPos.x+200, m_screenPos.y+200);
	m_scoreText.setPosition(m_screenPos.x + 20, m_screenPos.y + 650);
	m_score = 0;
	isDead = false;
	m_infoShowing = true;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (map[i][j] == nullptr);
			else
			{
				std::cout << "hej" << std::endl;
				delete(map[i][j]); //kallas inte ifall man stänger av arcademachine helt och startar igen, liten memory leak
			}
		}
	}

	direction = up;

	head = new stuff(turtle,5,5);
	head->before = nullptr;
	head->next = nullptr;
	map[5][5] = head;
	map[5][2] = new stuff(frie,5,2);
}

void Turtle::update(float deltaTime)
{

	time += deltaTime;

	if (!isDead)
	{
		if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Up")))
		{
			direction = up;
		}
		else if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Down")))
		{
			direction = down;
		}

		else if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Left")))
		{
			direction = left;
		}

		else if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Right")))
		{
			direction = right;
		}

		if (time > 1)
		{
			time--;
			int x = 0;
			int y = 0;
			if (direction = up)
			{
				y = 1;
			}
			else if (direction = down)
			{
				y = -1;
			}
			else if (direction = left)
			{
				x = -1;
			}
			else if (direction = right)
			{
				x = 1;
			}
			int newPosX = head->x + x;
			int newPosY = head->y + y;
			if (newPosX < 0 || newPosX > 9 || newPosY < 0 || newPosY > 9)
			{
				isDead = true;
			}
			else if (map[newPosX][newPosY] == nullptr)
			{
				stuff* ptr = head;
				while (ptr->next != nullptr)
				{

					ptr = ptr->next;
				}
				if(ptr->before != nullptr)ptr->before->next = nullptr;
				map[ptr->x][ptr->y] = nullptr;
				ptr->x = newPosX;
				ptr->y = newPosY;
				map[newPosX][newPosY] = ptr;
				if (ptr != head)
				{
					ptr->next = head;
					head = ptr;
				}
			}
			else if (map[head->x + x][head->y + y]->type == frie)
			{
				map[2][2] /* rand != turtle */ = map[newPosX][newPosY];
				stuff* ptr = new stuff(turtle, newPosX, newPosY);
				map[newPosX][newPosY] = ptr;
				ptr->next = head;
				ptr->before = nullptr;
				head->before = head;
				head = ptr;
			}
			else if (map[head->x + x][head->y + y]->type == turtle)
			{
				isDead = true;
			}
		}
		
		m_scoreText.setString("Score: " + std::to_string(m_score));
	}
	/*	m_scoreText.setPosition(m_screenPos.x + 180, m_screenPos.y + 320);
		m_scoreText.setString("Score: " + std::to_string(m_score) + '\n' + "Press ESC to quit");
*/
	
	m_turtleSprite.setTextureRect(m_turtleAnimation.getRectangle(deltaTime));
	m_frieSprite.setTextureRect(m_frieAnimation.getRectangle(deltaTime));
}

void Turtle::draw()
{
	Renderer::instance().drawHUD(m_background);
	if (m_infoShowing)
		Renderer::instance().drawHUD(m_infoText);

	

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (map[i][j] == nullptr);
			else if (map[i][j]->type == frie)
			{
				m_frieSprite.setPosition(m_screenPos + sf::Vector2f(i * 70, j * 70) );
				Renderer::instance().drawHUD(m_frieSprite);
			}
			else if (map[i][j]->type == turtle)
			{
				m_turtleSprite.setPosition(m_screenPos + sf::Vector2f(i * 70, j * 70));
				Renderer::instance().drawHUD(m_turtleSprite);
			}
		}
	}


	Renderer::instance().drawHUD(m_scoreText);
}