#include "BeachParty.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include "Utility.h"
#include "SoundPlayer.h"

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
	m_crabTexture = ResourceLoader::instance().retrieveTexture("crabsheet");
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

	m_infoText = sf::Text("Collect the Curly Fries", ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_infoText.setPosition(m_screenPos.x + 150, m_screenPos.y + 335);
	m_infoText.setColor(sf::Color(0, 120, 0, 255));

	m_scoreText.setColor(sf::Color(0,120,0,255));
}

sf::Vector2f randomPos(sf::Vector2f m_screenPos, sf::Vector2f turtlePos)
{
	sf::Vector2f pos = sf::Vector2f(Utility::randomValueBetween(m_screenPos.x + 20, m_screenPos.x + 680), Utility::randomValueBetween(m_screenPos.y + 20, m_screenPos.y + 680));

	while (abs(turtlePos.x - pos.x) + abs(turtlePos.y - pos.y) < 150)
	{
		pos = sf::Vector2f(Utility::randomValueBetween(m_screenPos.x + 20, m_screenPos.x + 680), Utility::randomValueBetween(m_screenPos.y + 20, m_screenPos.y + 680));
	}

	return pos;
}

void BeachParty::onGameStart()
{
	m_turtleSprite.setPosition(m_machine.getScreenPos().x + 325, m_machine.getScreenPos().y + 325);
	m_frieSprite.setPosition(sf::Vector2f(Utility::randomValueBetween(m_screenPos.x + 20, m_screenPos.x + 680), Utility::randomValueBetween(m_screenPos.y + 20, m_screenPos.y + 680)));
	m_scoreText.setPosition(m_machine.getScreenPos().x + 20, m_machine.getScreenPos().y + 650);
	m_crabs.clear();
	m_score = 0;
	m_isDead = false;
	m_infoShowing = true;
	m_rects.clear();
	m_rects.push_back(m_turtleSprite.getGlobalBounds());
	m_rects[0].width = 32;
	m_rects[0].height = 32;
	m_rects[0].left += 4;
	m_rects[0].top += 4;
}

void BeachParty::update(float deltaTime)
{
	SoundPlayer::instance().update(deltaTime, m_screenPos);

	if (!m_isDead)
	{
		m_newPos = sf::Vector2f(0, 0);

		m_rects[0].left = m_turtleSprite.getPosition().x;
		m_rects[0].top = m_turtleSprite.getPosition().y;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			m_newPos = sf::Vector2f(0, -deltaTime * m_speed);
			m_turtleSprite.setRotation(0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			m_newPos = sf::Vector2f(0, deltaTime * m_speed);
			m_turtleSprite.setRotation(180);
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			m_newPos = sf::Vector2f(-deltaTime * m_speed, 0);
			m_turtleSprite.setRotation(270);
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			m_newPos = sf::Vector2f(deltaTime * m_speed, 0);
			m_turtleSprite.setRotation(90);
		}


		float x = m_turtleSprite.getPosition().x + m_newPos.x;
		float y = m_turtleSprite.getPosition().y + m_newPos.y;

		if (x > m_machine.getScreenPos().x + 25 && x < m_machine.getScreenPos().x + 675 && y > m_machine.getScreenPos().y + 25 && y < m_machine.getScreenPos().y + 675)
			m_turtleSprite.setPosition(m_turtleSprite.getPosition() + m_newPos);

		if (abs(m_turtleSprite.getPosition().x - m_frieSprite.getPosition().x) + abs(m_turtleSprite.getPosition().y - m_frieSprite.getPosition().y) < 40)
		{
			m_frieSprite.setPosition(randomPos(m_screenPos, m_turtleSprite.getPosition()));
			m_score++;

			m_crabs.push_back(sf::Sprite(m_crabTexture));
			m_rects.push_back(sf::FloatRect(-100, -100, 40, 40));
			m_crabs.back().setPosition(randomPos(m_screenPos, m_turtleSprite.getPosition()));
			m_crabDirections.push_back(true);
			m_infoShowing = false;

			SoundPlayer::instance().playSound("ArcadeLight2", m_screenPos, 10);
		}
		sf::IntRect crabby = m_crabAnimation.getRectangle(deltaTime);
		for (int i = 0; i < m_crabs.size(); i++)
		{
			for (int i = 1; i < m_rects.size(); i++)
			{
				m_rects[i].left = m_crabs[i - 1].getPosition().x;
				m_rects[i].top = m_crabs[i - 1].getPosition().y;
			}

			sf::Vector2f newCrabPos;

			m_crabs[i].setOrigin(20, 20);
			m_crabs[i].setTextureRect(crabby);

			if (i % 2 == 0)
			{
				m_crabs[i].setRotation(90);
				if (m_crabDirections[i])
					newCrabPos = sf::Vector2f(0, m_speed);
				else
					newCrabPos = sf::Vector2f(0, -m_speed);
			}
			else
			{
				if (m_crabDirections[i])
					newCrabPos = sf::Vector2f(m_speed, 0);
				else
					newCrabPos = sf::Vector2f(-m_speed, 0);
			}

			float x = m_crabs[i].getPosition().x + deltaTime *newCrabPos.x;
			float y = m_crabs[i].getPosition().y + deltaTime *newCrabPos.y;

			if (x > m_machine.getScreenPos().x + 20 && x < m_machine.getScreenPos().x + 680 && y > m_machine.getScreenPos().y + 20 && y < m_machine.getScreenPos().y + 680)
			{
				m_crabs[i].setPosition(m_crabs[i].getPosition() + deltaTime * newCrabPos);
			}
			else
				m_crabDirections[i] = !m_crabDirections[i];

			if (m_rects[0].intersects(m_rects[i + 1]) && abs(m_turtleSprite.getPosition().x - m_crabs[i].getPosition().x) + abs(m_turtleSprite.getPosition().y - m_crabs[i].getPosition().y) < 40)
			{
				m_isDead = true;
				SoundPlayer::instance().playSound("ArcadeFail", m_screenPos, 10);
			}
		}
		m_scoreText.setString("Score: " + std::to_string(m_score));
	}
	else
	{
		m_scoreText.setPosition(m_screenPos.x + 180, m_screenPos.y + 320);
		m_scoreText.setString("Score: " + std::to_string(m_score) + '\n' + "Press ESC to quit");
	}

	
	m_turtleSprite.setTextureRect(m_turtleAnimation.getRectangle(deltaTime));
	m_frieSprite.setTextureRect(m_frieAnimation.getRectangle(deltaTime));
}

void BeachParty::draw()
{
	Renderer::instance().drawHUD(m_background);
	if (m_infoShowing)
	Renderer::instance().drawHUD(m_infoText);
	Renderer::instance().drawHUD(m_turtleSprite);
	Renderer::instance().drawHUD(m_frieSprite);

	for (int i = 0; i < m_crabs.size(); i++)
	{
		Renderer::instance().drawHUD(m_crabs[i]);
	}

	Renderer::instance().drawHUD(m_scoreText);
}