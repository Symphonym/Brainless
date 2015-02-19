#include "NoteGame.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include "Utility.h"

NoteGame::NoteGame(ArcadeMachine &machine)
:
ArcadeGame(machine, "Åkes Melodisak"),
m_score(0),
m_speedAddition(0)
{
	m_scoreText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_scoreText.setPosition(m_machine.getScreenPos().x, m_machine.getScreenPos().y + m_machine.getScreenSize().y - 100.f);
	m_scoreText.setString("Score: 0");

	m_baseANote.setTexture(ResourceLoader::instance().retrieveTexture("RedArcadeNote"));
	m_baseWNote.setTexture(ResourceLoader::instance().retrieveTexture("GreenArcadeNote"));
	m_baseDNote.setTexture(ResourceLoader::instance().retrieveTexture("BlueArcadeNote"));

	m_baseANote.setOrigin(
		m_baseANote.getGlobalBounds().width / 2.f,
		m_baseANote.getGlobalBounds().height / 2.f);
	m_baseWNote.setOrigin(
		m_baseWNote.getGlobalBounds().width / 2.f,
		m_baseWNote.getGlobalBounds().height / 2.f);
	m_baseDNote.setOrigin(
		m_baseDNote.getGlobalBounds().width / 2.f,
		m_baseDNote.getGlobalBounds().height / 2.f);

	float machineMidX = m_machine.getScreenPos().x + m_machine.getScreenSize().x / 2.f;
	m_baseANote.setPosition(machineMidX - 100.f, m_machine.getScreenPos().y + m_machine.getScreenSize().y - 100.f);
	m_baseWNote.setPosition(machineMidX, m_machine.getScreenPos().y + m_machine.getScreenSize().y - 100.f);
	m_baseDNote.setPosition(machineMidX + 100.f, m_machine.getScreenPos().y + m_machine.getScreenSize().y - 100.f);
}

void NoteGame::onGameStart()
{
	m_speedAddition = 0;
	m_aNotes.clear();
	m_wNotes.clear();
	m_dNotes.clear();
}

void NoteGame::events(const sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::A)
			m_baseANote.setScale(1.5f, 1.5f);
		else if (event.key.code == sf::Keyboard::W)
			m_baseWNote.setScale(1.5f, 1.5f);
		else if (event.key.code == sf::Keyboard::D)
			m_baseDNote.setScale(1.5f, 1.5f);
	}
	else if (event.type == sf::Event::KeyReleased)
	{

		if (event.key.code == sf::Keyboard::A)
		{
			bool noScore = true;
			m_baseANote.setScale(1, 1);
			for (auto itr = m_aNotes.begin(); itr != m_aNotes.end();)
			{
				float distance = m_machine.getScreenPos().y + m_machine.getScreenSize().y - 100.f - itr->getPosition().y;
				if (handleDistanceScore(distance))
				{
					itr = m_aNotes.erase(itr);
					noScore = false;
					break;
				}
				else
					++itr;
			}

			if (noScore)
				penalty();
		}
		else if (event.key.code == sf::Keyboard::W)
		{
			m_baseWNote.setScale(1, 1);
			bool noScore = true;
			for (auto itr = m_wNotes.begin(); itr != m_wNotes.end();)
			{
				float distance = m_machine.getScreenPos().y + m_machine.getScreenSize().y - 100.f - itr->getPosition().y;
				if (handleDistanceScore(distance))
				{
					itr = m_wNotes.erase(itr);
					noScore = false;
					break;
				}
				else
					++itr;
			}

			if (noScore)
				penalty();
		}
		else if (event.key.code == sf::Keyboard::D)
		{
			m_baseDNote.setScale(1, 1);
			bool noScore = true;
			for (auto itr = m_dNotes.begin(); itr != m_dNotes.end();)
			{
				float distance = m_machine.getScreenPos().y + m_machine.getScreenSize().y - 100.f - itr->getPosition().y;
				if (handleDistanceScore(distance))
				{
					itr = m_dNotes.erase(itr);
					noScore = false;
					break;
				}
				else
					++itr;
			}

			if (noScore)
				penalty();
		}
	}
}
void NoteGame::update(float deltaTime)
{
	m_scoreText.setString("Score: " + std::to_string(m_score));
	m_speedAddition += deltaTime*10.f;

	float machineMidX = m_machine.getScreenPos().x + m_machine.getScreenSize().x / 2.f;

	m_aCurDelay += deltaTime;
	m_wCurDelay += deltaTime;
	m_dCurDelay += deltaTime;

	// Spawn new notes
	if (m_aCurDelay >= m_aMaxDelay)
	{
		m_aCurDelay = m_aCurDelay - m_aMaxDelay;
		
		sf::Sprite aNote;
		aNote.setTexture(ResourceLoader::instance().retrieveTexture("RedArcadeNote"));
		aNote.setPosition(machineMidX - 100.f - aNote.getGlobalBounds().width/2.f, 0);
		m_aNotes.push_back(aNote);

		m_aMaxDelay = Utility::randomValueBetween(0.8f, 1.2f);
	}
	if (m_wCurDelay >= m_wMaxDelay)
	{
		m_wCurDelay = m_wCurDelay - m_wMaxDelay;

		sf::Sprite wNote;
		wNote.setTexture(ResourceLoader::instance().retrieveTexture("GreenArcadeNote"));
		wNote.setPosition(machineMidX - wNote.getGlobalBounds().width / 2.f, 0);
		m_wNotes.push_back(wNote);

		m_wMaxDelay = Utility::randomValueBetween(0.4f, 1.f);
	}
	if (m_dCurDelay >= m_dMaxDelay)
	{
		m_dCurDelay = m_dCurDelay - m_dMaxDelay;

		sf::Sprite dNote;
		dNote.setTexture(ResourceLoader::instance().retrieveTexture("BlueArcadeNote"));
		dNote.setPosition(machineMidX + 100.f - dNote.getGlobalBounds().width / 2.f, 0);
		m_dNotes.push_back(dNote);

		m_dMaxDelay = Utility::randomValueBetween(0.6f, 1.f);
	}

	// Check for out of bounds and update
	for (auto itr = m_aNotes.begin(); itr != m_aNotes.end();)
	{
		itr->move(0, (200.f + m_speedAddition)*deltaTime);

		// TODO ADD FAIL TEXT
		if (itr->getPosition().y > m_machine.getScreenPos().y + m_machine.getScreenSize().y)
		{
			itr = m_aNotes.erase(itr);
			penalty();
		}
		else
			++itr;
	}
	for (auto itr = m_wNotes.begin(); itr != m_wNotes.end();)
	{
		itr->move(0, (240.f + m_speedAddition)*deltaTime);

		// TODO ADD FAIL TEXT
		if (itr->getPosition().y > m_machine.getScreenPos().y + m_machine.getScreenSize().y)
		{
			itr = m_wNotes.erase(itr);
			penalty();
		}
		else
			++itr;
	}
	for (auto itr = m_dNotes.begin(); itr != m_dNotes.end();)
	{
		itr->move(0, (220.f + m_speedAddition)*deltaTime);

		// TODO ADD FAIL TEXT
		if (itr->getPosition().y > m_machine.getScreenPos().y + m_machine.getScreenSize().y)
		{
			itr = m_dNotes.erase(itr);
			penalty();
		}
		else
			++itr;
	}
}
void NoteGame::draw()
{
	Renderer::instance().drawHUD(m_scoreText);

	Renderer::instance().drawHUD(m_baseANote);
	Renderer::instance().drawHUD(m_baseWNote);
	Renderer::instance().drawHUD(m_baseDNote);

	for (auto &note : m_aNotes)
		Renderer::instance().drawHUD(note);
	for (auto &note : m_wNotes)
		Renderer::instance().drawHUD(note);
	for (auto &note : m_dNotes)
		Renderer::instance().drawHUD(note);
}

bool NoteGame::handleDistanceScore(float distance)
{
	distance = std::abs(distance);

	// Perfect
	if (distance <= 5)
	{
		m_score += 50;
		return true;
	}
	// Good
	else if (distance <= 15)
	{
		m_score += 20;
		return true;
	}
	// Decent
	else if (distance <= 50)
	{
		m_score += 5;
		return true;
	}
	else
		return false;
}

void NoteGame::penalty()
{
	m_score -= 50;

	if (m_score < 0)
		m_score = 0;
}