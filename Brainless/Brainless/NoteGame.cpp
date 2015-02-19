#include "NoteGame.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include "Utility.h"
#include "ParticleSystem.h"

NoteGame::NoteGame(ArcadeMachine &machine)
:
ArcadeGame(machine, "Åkes Melodisak"),
m_score(0),
m_noteSpeed(500),
m_curDelay(0),
m_maxDelay(0),
m_spawnString("000")
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
	m_noteSpeed = 500;
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
			m_baseANote.setScale(1, 1);
			validateNoteInput(m_aNotes, m_baseANote);
		}
		else if (event.key.code == sf::Keyboard::W)
		{
			m_baseWNote.setScale(1, 1);
			validateNoteInput(m_wNotes, m_baseWNote);
		}
		else if (event.key.code == sf::Keyboard::D)
		{
			m_baseDNote.setScale(1, 1);
			validateNoteInput(m_dNotes, m_baseDNote);
		}
	}
}
void NoteGame::update(float deltaTime)
{
	m_scoreText.setString("Score: " + std::to_string(m_score));
	//m_noteSpeed += deltaTime*10.f;
	ParticleSystem::instance().update(deltaTime);

	m_curDelay += deltaTime;

	int textCount = 0;
	for (auto itr = m_hitTexts.begin(); itr != m_hitTexts.end();)
	{
		itr->first -= deltaTime;

		if (itr->first <= 0)
			itr = m_hitTexts.erase(itr);
		else
		{
			itr->second.setPosition(
				m_machine.getScreenPos().x + 100.f,
				m_machine.getScreenPos().y + m_machine.getScreenSize().y / 2.f + 20*textCount);
			sf::Color col = itr->second.getColor();
			col.a = itr->first * 255;
			itr->second.setColor(col);
			++itr;
		}

		++textCount;
	}

	// Spawn new notes
	if (m_curDelay >= m_maxDelay)
	{
		float machineMidX = m_machine.getScreenPos().x + m_machine.getScreenSize().x / 2.f;
		m_curDelay = m_curDelay - m_maxDelay;
		
		if (m_spawnString[0] == '1')
		{
			sf::Sprite aNote;
			aNote.setTexture(ResourceLoader::instance().retrieveTexture("RedArcadeNote"));
			aNote.setPosition(machineMidX - 100.f - aNote.getGlobalBounds().width / 2.f, 0);
			m_aNotes.push_back(aNote);
		}
		if (m_spawnString[1] == '1')
		{
			sf::Sprite wNote;
			wNote.setTexture(ResourceLoader::instance().retrieveTexture("GreenArcadeNote"));
			wNote.setPosition(machineMidX - wNote.getGlobalBounds().width / 2.f, 0);
			m_wNotes.push_back(wNote);
		}
		if (m_spawnString[2] == '1')
		{
			sf::Sprite dNote;
			dNote.setTexture(ResourceLoader::instance().retrieveTexture("BlueArcadeNote"));
			dNote.setPosition(machineMidX + 100.f - dNote.getGlobalBounds().width / 2.f, 0);
			m_dNotes.push_back(dNote);
		}



		auto randomBinaryStr = []() -> std::string
		{
			return std::to_string(std::rand() % 2);
		};
		m_spawnString = randomBinaryStr() + randomBinaryStr() + randomBinaryStr();
		m_maxDelay = Utility::randomValueBetween(0.15f, 0.6f);
	}

	// Check for out of bounds and update
	for (auto itr = m_aNotes.begin(); itr != m_aNotes.end();)
	{
		itr->move(0, m_noteSpeed*deltaTime);

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
		itr->move(0, m_noteSpeed*deltaTime);

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
		itr->move(0, m_noteSpeed*deltaTime);

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

	ParticleSystem::instance().draw(true);

	for (auto &note : m_aNotes)
		Renderer::instance().drawHUD(note);
	for (auto &note : m_wNotes)
		Renderer::instance().drawHUD(note);
	for (auto &note : m_dNotes)
		Renderer::instance().drawHUD(note);

	for (auto &text : m_hitTexts)
		Renderer::instance().drawHUD(text.second);
}

void NoteGame::validateNoteInput(std::vector<sf::Sprite> &noteList, const sf::Sprite &baseNote)
{
	bool noScore = true;
	for (auto itr = noteList.begin(); itr != noteList.end();)
	{
		float distance = m_machine.getScreenPos().y + m_machine.getScreenSize().y - 100.f - itr->getPosition().y;
		if (handleDistanceScore(distance))
		{
			ParticleSystem::instance().addParticles(50, baseNote.getPosition(), sf::Color::Green);
			itr = noteList.erase(itr);
			noScore = false;
			break;
		}
		else
			++itr;
	}

	if (noScore)
		penalty();
}
void NoteGame::createHitText(const std::string &text, const sf::Color &color)
{
	sf::Text newText;
	newText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	newText.setColor(color);
	newText.setString(text);
	m_hitTexts.push_back(std::make_pair(2, newText));
}

bool NoteGame::handleDistanceScore(float distance)
{
	distance = std::abs(distance);

	// Perfect
	if (distance <= 5)
	{
		createHitText("PERFECT", sf::Color::Green);
		m_score += 50;
		return true;
	}
	// Good
	else if (distance <= 15)
	{
		createHitText("GOOD", sf::Color::Cyan);
		m_score += 20;
		return true;
	}
	// Decent
	else if (distance <= 50)
	{
		createHitText("DECENT", sf::Color::Yellow);
		m_score += 5;
		return true;
	}
	else
	{
		return false;
	}
}

void NoteGame::penalty()
{
	createHitText("FAIL", sf::Color::Red);
	m_score -= 50;

	if (m_score < 0)
		m_score = 0;
}