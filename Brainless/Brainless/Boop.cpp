#include "Boop.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include "SoundPlayer.h"

Boop::Boop(ArcadeMachine &machine)
:
ArcadeGame(machine, "Boop Simulator"),
m_screenPos(m_machine.getScreenPos()),
m_score(0)
{
	m_background.setTexture(ResourceLoader::instance().retrieveTexture("boopbg"));
	m_background.setPosition(m_machine.getScreenPos());

	m_cloud1.setTexture(ResourceLoader::instance().retrieveTexture("cloud"));
	m_cloud2.setTexture(ResourceLoader::instance().retrieveTexture("cloud"));

	m_cloud1.setPosition(m_machine.getScreenPos().x + 200, m_machine.getScreenPos().y + 50);
	m_cloud2.setPosition(m_machine.getScreenPos().x + 450, m_machine.getScreenPos().y + 50);

	m_cloud2.setScale(sf::Vector2f(0.7f, 0.7f));

	m_hand1.setTexture(ResourceLoader::instance().retrieveTexture("hand1"));
	m_hand2.setTexture(ResourceLoader::instance().retrieveTexture("hand2"));

	m_originalHandPos = sf::Vector2f(m_screenPos.x + 700, m_screenPos.y + 385);

	m_hand1.setPosition(m_originalHandPos);
	m_hand2.setPosition(m_originalHandPos);

	m_scoreText = sf::Text("Score: " + m_score, ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_scoreText.setColor(sf::Color(0, 120, 0, 255));
}

void Boop::onGameStart()
{
	m_scoreText.setPosition(m_machine.getScreenPos().x + 20, m_machine.getScreenPos().y + 650);
	m_score = 0;

	m_hand1.setPosition(m_originalHandPos);
	m_hand2.setPosition(m_originalHandPos);

	m_isSoundPlaying = false;

	SoundPlayer::instance().stopMusic("ArcadeMusic");
}

void Boop::update(float deltaTime)
{
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && !m_isBooped)
	{
		m_hand1.setPosition(m_hand1.getPosition() + sf::Vector2f(-deltaTime * m_speed, 0));

		if (!m_isSoundPlaying)
		SoundPlayer::instance().playSound("Boop", m_screenPos, 100);

		m_isSoundPlaying = true;
	}
	else
	{
		if (m_hand1.getPosition().x < m_screenPos.x + 700 && !m_isBooped)
		{
			m_hand1.setPosition(m_hand1.getPosition() + sf::Vector2f(deltaTime * m_speed * 10, 0));
			m_isBooped = true;
			SoundPlayer::instance().killAllSounds();
		}
	}

	if (m_isBooped)
	{
		m_hand1.setPosition(m_hand1.getPosition() + sf::Vector2f(deltaTime * m_speed * 10, 0));
	}

	if (m_hand1.getPosition().x > m_originalHandPos.x)
	{
		m_hand1.setPosition(m_originalHandPos.x, m_hand1.getPosition().y);
		SoundPlayer::instance().killAllSounds();
	}

	if (m_hand1.getPosition().x < m_originalHandPos.x - 160)
		m_hand2.setPosition(m_hand1.getPosition() + sf::Vector2f(160, 0));
	else
	{
		m_hand2.setPosition(m_originalHandPos);

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S)
			&& !sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			m_isBooped = false;
			m_isSoundPlaying = false;
		}
	}

	if (m_hand1.getPosition().x < m_screenPos.x + m_boopPos)
	{
		m_score++;
		m_isBooped = true;
	}

	m_cloud1.setPosition(m_cloud1.getPosition() + sf::Vector2f(-deltaTime * 13, 0));
	m_cloud2.setPosition(m_cloud2.getPosition() + sf::Vector2f(-deltaTime * 8, 0));

	if (m_cloud1.getPosition().x < m_screenPos.x - 159)
	{
		m_cloud1.setPosition(m_screenPos.x + 700, m_cloud1.getPosition().y);
	}

	if (m_cloud2.getPosition().x < m_screenPos.x - 159)
	{
		m_cloud2.setPosition(m_screenPos.x + 700, m_cloud2.getPosition().y);
	}

	m_scoreText.setString("Score: " + std::to_string(m_score));
}

void Boop::draw()
{
	Renderer::instance().drawHUD(m_background);
	Renderer::instance().drawHUD(m_cloud1);
	Renderer::instance().drawHUD(m_cloud2);
	Renderer::instance().drawHUD(m_hand1);
	Renderer::instance().drawHUD(m_hand2);

	Renderer::instance().drawHUD(m_scoreText);
}