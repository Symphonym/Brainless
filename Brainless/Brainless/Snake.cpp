#include "Snake.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include "Utility.h"
#include "SoundPlayer.h"

Snake::Snake(ArcadeMachine &machine)
:
ArcadeGame(machine, "Snake"),
m_turtleAnimation(50, 50),
m_frieAnimation(40, 40),
m_screenPos(m_machine.getScreenPos()),
m_score(0)
{
	m_turtleSprite.setTexture(ResourceLoader::instance().retrieveTexture("turtlesheet"));
	m_frieSprite.setTexture(ResourceLoader::instance().retrieveTexture("friesheet"));
	m_background.setTexture(ResourceLoader::instance().retrieveTexture("snakeBackground"));

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
}


void Snake::onGameStart()
{
	m_turtleSprite.setPosition(m_screenPos.x+300, m_screenPos.y+300);
	m_frieSprite.setPosition(m_screenPos.x+200, m_screenPos.y+200);
	m_scoreText.setPosition(m_screenPos.x + 20, m_screenPos.y + 650);
	m_score = 0;
	m_isDead = false;
	m_infoShowing = true;
}

void Snake::update(float deltaTime)
{

	if (!m_isDead)
	{
		


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{

		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{

		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{

		}



		m_scoreText.setString("Score: " + std::to_string(m_score));
	}
		m_scoreText.setPosition(m_screenPos.x + 180, m_screenPos.y + 320);
		m_scoreText.setString("Score: " + std::to_string(m_score) + '\n' + "Press ESC to quit");

	
	m_turtleSprite.setTextureRect(m_turtleAnimation.getRectangle(deltaTime));
	m_frieSprite.setTextureRect(m_frieAnimation.getRectangle(deltaTime));
}

void Snake::draw()
{
	Renderer::instance().drawHUD(m_background);
	if (m_infoShowing)
		Renderer::instance().drawHUD(m_infoText);
	Renderer::instance().drawHUD(m_turtleSprite);
	Renderer::instance().drawHUD(m_frieSprite);

	Renderer::instance().drawHUD(m_scoreText);
}