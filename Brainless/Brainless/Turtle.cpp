#include <iostream>
#include "Turtle.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include "Utility.h"
#include "SoundPlayer.h"
#include "OptionsMenu.h"

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
	m_isDead = false;
	m_infoShowing = true;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (map[i][j] == nullptr);
			else
			{
				std::cout << "hej" << std::endl;
				delete(map[i][j]); //kallas inte ifall man stänger av arcademachine helt och startar igen, destructor körs ej?
			}
		}
	}

	
	head = new stuff(turtle);
	map[5][5] = head;
	map[5][2] = new stuff(frie);
}

void Turtle::update(float deltaTime)
{

	map[0][0] = new stuff(turtle);

	map[9][9] = new stuff(frie);



	if (!m_isDead)
	{
		


		if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Up")))
		{

		}
		else if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Down")))
		{

		}

		else if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Left")))
		{

		}

		else if (sf::Keyboard::isKeyPressed(OptionsMenu::getKeybind("Right")))
		{

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