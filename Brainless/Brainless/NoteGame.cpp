#include "NoteGame.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"
#include "Utility.h"
#include "ParticleSystem.h"
#include "SoundPlayer.h"

const float NoteGame::HitTextDuration = 2.f;

NoteGame::NoteGame(ArcadeMachine &machine)
:
ArcadeGame(machine, "Åkes Melodisak"),
m_gameOver(false),
m_score(0),
m_noteSpeed(500),
m_curDelay(0),
m_maxDelay(0),
m_spawnString("000"),
m_health(MaxHealth)
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

	m_background.setTexture(ResourceLoader::instance().retrieveTexture("MusicArcadeBG"));
	m_background.setPosition(m_machine.getScreenPos());

	sf::Image failImg;
	failImg.create(1, 25, sf::Color::White);
	
	m_failBarTexture.loadFromImage(failImg);

	m_failBar.setTexture(m_failBarTexture);
	m_failBar.setPosition(
		m_machine.getScreenPos().x,
		m_machine.getScreenPos().y + m_machine.getScreenSize().y - m_failBar.getGlobalBounds().height);


	m_gameOverText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_gameOverText.setString("             Game over\npress ESC to return to the main menu");
	m_gameOverText.setPosition(
		m_machine.getScreenPos().x + m_machine.getScreenSize().x / 2 - m_gameOverText.getGlobalBounds().width / 2.f,
		m_machine.getScreenPos().y + m_machine.getScreenSize().y / 2 - m_gameOverText.getGlobalBounds().height / 2.f);
}

void NoteGame::onGameStart()
{
	m_gameOver = false;
	m_health = MaxHealth;
	m_noteSpeed = 500;
	m_score = 0;
	m_aNotes.clear();
	m_wNotes.clear();
	m_dNotes.clear();
	m_hitTexts.clear();
}

void NoteGame::events(const sf::Event &event)
{
	if (m_gameOver)
		return;

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::A)
			m_baseANote.setScale(1.5f, 1.5f);
		else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S)
			m_baseWNote.setScale(1.5f, 1.5f);
		else if (event.key.code == sf::Keyboard::D)
			m_baseDNote.setScale(1.5f, 1.5f);
	}
	else if (event.type == sf::Event::KeyReleased)
	{

		if (event.key.code == sf::Keyboard::A)
		{
			m_baseANote.setScale(1, 1);
			if(validateNoteInput(m_aNotes, m_baseANote))
				SoundPlayer::instance().playSound("ArcadeLight", m_machine.getScreenPos(), 30.f);

		}
		else if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S)
		{
			m_baseWNote.setScale(1, 1);
			if(validateNoteInput(m_wNotes, m_baseWNote))
				SoundPlayer::instance().playSound("ArcadeMedium", m_machine.getScreenPos(), 30.f);
		}
		else if (event.key.code == sf::Keyboard::D)
		{
			m_baseDNote.setScale(1, 1);
			if(validateNoteInput(m_dNotes, m_baseDNote))
				SoundPlayer::instance().playSound("ArcadeDark", m_machine.getScreenPos(), 30.f);
		}
	}
}
void NoteGame::update(float deltaTime)
{
	m_scoreText.setString("Score: " + std::to_string(m_score));
	ParticleSystem::instance().update(deltaTime);

	if (m_gameOver)
		return;

	if (m_health <= 0)
	{
		m_gameOver = true;

	}

	m_curDelay += deltaTime;

	int textCount = 0;
	for (int i = m_hitTexts.size()-1; i >= 0; i--)
	{
		auto &hitText = m_hitTexts[i];
		hitText.first -= deltaTime;

		hitText.second.setPosition(
			m_machine.getScreenPos().x + 60.f,
			m_machine.getScreenPos().y + m_machine.getScreenSize().y / 2.f + 20 * textCount);
		sf::Color col = hitText.second.getColor();
		col.a = (hitText.first / HitTextDuration) * 255;
		hitText.second.setColor(col);
		++textCount;

	}

	m_hitTexts.erase(std::remove_if(m_hitTexts.begin(), m_hitTexts.end(), [](const std::pair<float, sf::Text>& hitText) -> bool
	{
		return hitText.first <= 0;
	}), m_hitTexts.end());

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
		m_maxDelay = Utility::randomValueBetween(0.15f, 0.4f);
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

	m_health = Utility::clampValue<int>(m_health, 0, MaxHealth);
	m_failBar.setScale(
		(static_cast<float>(m_health) / static_cast<float>(MaxHealth)) * m_machine.getScreenSize().x,
		1);
}
void NoteGame::draw()
{
	Renderer::instance().drawHUD(m_background);
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

	Renderer::instance().drawHUD(m_failBar);

	if (m_gameOver)
		Renderer::instance().drawHUD(m_gameOverText);
}

bool NoteGame::validateNoteInput(std::vector<sf::Sprite> &noteList, const sf::Sprite &baseNote)
{
	bool noScore = true;
	for (auto itr = noteList.begin(); itr != noteList.end();)
	{
		float distance = m_machine.getScreenPos().y + m_machine.getScreenSize().y - 100.f - itr->getPosition().y;
		if (handleDistanceScore(distance))
		{
			ParticleSystem::instance().addParticles(50, baseNote.getPosition(), sf::Color::Green,
				sf::Vector2f(0.2f, 0.6f), sf::Vector2f(0, 360), sf::Vector2f(0, 100));
			itr = noteList.erase(itr);
			noScore = false;
			break;
		}
		else
			++itr;
	}

	if (noScore)
	{
		penalty();
		return false;
	}
	else
		return true;
}
void NoteGame::createHitText(const std::string &text, const sf::Color &color)
{
	sf::Text newText;
	newText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	newText.setColor(color);
	newText.setString(text);
	m_hitTexts.push_back(std::make_pair(HitTextDuration, newText));

	if (m_hitTexts.size() >= 12)
		m_hitTexts.erase(m_hitTexts.begin());
}

bool NoteGame::handleDistanceScore(float distance)
{
	distance = std::abs(distance);

	// Perfect
	if (distance <= 5)
	{
		m_health += 5;
		createHitText("PERFECT", sf::Color::Green);
		m_score += 50;
		return true;
	}
	// Good
	else if (distance <= 15)
	{
		m_health += 3;
		createHitText("GOOD", sf::Color::Cyan);
		m_score += 20;
		return true;
	}
	// Decent
	else if (distance <= 50)
	{
		m_health += 1;
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
	SoundPlayer::instance().playSound("ArcadeFail", m_machine.getScreenPos(), 50.f);
	m_health -= 10;

	if (m_score < 0)
		m_score = 0;
}