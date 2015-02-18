#include "BeachParty.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include "Utility.h"

BeachParty::BeachParty(ArcadeMachine &machine)
:
ArcadeGame(machine, "Åke's Beach Party"),
m_turtleAnimation(50,50),
m_crabAnimation(40,40),
m_frieAnimation(40,40),
m_screenPos(m_machine.getScreenPos()),
m_score(0)
{
	m_turtleSprite.setTexture(ResourceLoader::instance().retrieveTexture("turtlesheet"));
	m_crabTexture.setTexture(ResourceLoader::instance().retrieveTexture("crabsheet"));
	m_frieSprite.setTexture(ResourceLoader::instance().retrieveTexture("friesheet"));
	m_background.setTexture(ResourceLoader::instance().retrieveTexture("beach"));

	m_turtleAnimation.loop(0, 1, 0, 1.5f);
	m_crabAnimation.loop(0, 1, 0, 1.5f);
	m_frieAnimation.loop(0, 1, 0, 1.5f);

	m_turtleSprite.setPosition(m_machine.getScreenPos().x + 325, m_machine.getScreenPos().y + 325);
	m_turtleSprite.setOrigin(sf::Vector2f(25, 25));

	m_frieSprite.setOrigin(sf::Vector2f(20, 20));

	m_background.setPosition(m_machine.getScreenPos());

	m_frieSprite.setPosition(sf::Vector2f(Utility::randomValueBetween(m_screenPos.x + 20, m_screenPos.x + 680), Utility::randomValueBetween(m_screenPos.y + 20, m_screenPos.y + 680)));

	m_scoreText = sf::Text("Score: " + m_score, ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_scoreText.setPosition(m_machine.getScreenPos().x + 20, m_machine.getScreenPos().y + 650);
	m_scoreText.setColor(sf::Color::Black);
}

sf::Vector2f randomPos(sf::Vector2f m_screenPos)
{
	return sf::Vector2f(Utility::randomValueBetween(m_screenPos.x + 20, m_screenPos.x + 680), Utility::randomValueBetween(m_screenPos.y + 20, m_screenPos.y + 680));
}

void BeachParty::update(float deltaTime)
{
	m_newPos = sf::Vector2f(0, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_newPos = sf::Vector2f(0, -deltaTime * 100);
		m_turtleSprite.setRotation(0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_newPos = sf::Vector2f(0, deltaTime * 100);
		m_turtleSprite.setRotation(180);
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_newPos = sf::Vector2f(-deltaTime * 100, 0);
		m_turtleSprite.setRotation(270);
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_newPos = sf::Vector2f(deltaTime * 100, 0);
		m_turtleSprite.setRotation(90);
	}


	float x = m_turtleSprite.getPosition().x + m_newPos.x;
	float y = m_turtleSprite.getPosition().y + m_newPos.y;

	if (x > m_machine.getScreenPos().x + 25 && x < m_machine.getScreenPos().x + 675 && y > m_machine.getScreenPos().y + 25 && y < m_machine.getScreenPos().y + 675)
	m_turtleSprite.setPosition(m_turtleSprite.getPosition() + m_newPos);

	if (abs(m_turtleSprite.getPosition().x - m_frieSprite.getPosition().x) + abs(m_turtleSprite.getPosition().y - m_frieSprite.getPosition().y) < 40)
	{
		m_frieSprite.setPosition(randomPos(m_screenPos));
		m_score++;
		while(abs(m_turtleSprite.getPosition().x - m_frieSprite.getPosition().x) + abs(m_turtleSprite.getPosition().y - m_frieSprite.getPosition().y) < 40)
		{
			m_frieSprite.setPosition(randomPos(m_screenPos));
		}
	}

	m_scoreText.setString("Score: " + std::to_string(m_score));
	m_turtleSprite.setTextureRect(m_turtleAnimation.getRectangle(deltaTime));
	m_frieSprite.setTextureRect(m_frieAnimation.getRectangle(deltaTime));
}

void BeachParty::draw()
{
	Renderer::instance().drawHUD(m_background);
	Renderer::instance().drawHUD(m_turtleSprite);
	Renderer::instance().drawHUD(m_frieSprite);
	Renderer::instance().drawHUD(m_scoreText);
}