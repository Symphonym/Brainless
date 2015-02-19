#include "ArcadeMachine.h"
#include "Renderer.h"
#include "StateMachine.h"
#include "Utility.h"
#include "ArcadeGame.h"
#include "ResourceLoader.h"
#include "BeachParty.h"

#include "NoteGame.h"
#include "RobotAkeAttack.h"

ArcadeMachine::ArcadeMachine(StateMachine &machine)
:
State(machine),
m_currentGameIndex(0),
m_playingGame(false)
{
	// Load game resources
	ResourceLoader::instance().loadResourceFile("loadfiles/ResourceLoad_ArcadeMachine.txt");

	m_arcadeBackground.setTexture(ResourceLoader::instance().retrieveTexture("ArcadeMachineScreen"));
	//m_arcadeBackground.setPosition(
	//	m_window.getSize().x / 2.f - m_arcadeBackground.getGlobalBounds().width / 2.f,
	//	0);

	// ADD YOUR GAMES HERE
	m_games[0] = GamePtr(new BeachParty(*this));
	//m_games[1] = GamePtr(new ...);
	//m_games[0] = GamePtr(new ...);
	m_games[1] = GamePtr(new NoteGame(*this));
	m_games[2] = GamePtr(new RobotAkeAttack(*this));
	//m_games[2] = GamePtr(new ...);
	//m_games[3] = GamePtr(new ...);
	//m_games[4] = GamePtr(new ...);

	m_currentGameText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_currentGameText.setPosition(getScreenPos());
	m_currentGameText.setColor(sf::Color(0, 120, 0, 255));

	for (std::size_t i = 0; i < m_gameSelectionButtons.size(); i++)
	{
		sf::Sprite &button = m_gameSelectionButtons[i];
		button.setTexture(ResourceLoader::instance().retrieveTexture("ArcadeMenuButton"));
		button.setPosition(
			getScreenPos().x + getScreenSize().x / 2.f - button.getGlobalBounds().width / 2.f,
			getScreenPos().y + 100 + (button.getGlobalBounds().height + 5.f)*i);
	}

	for (std::size_t i = 0; i < GameCount - 1; i++)
	{
		//m_gameNames[i] = sf::Text(m_games[i]->getName(), ResourceLoader::instance().retrieveFont("DefaultFont"));
		/*m_gameNames[i].setColor(sf::Color(0, 120, 0, 255));
		m_gameNames[i].setPosition(getScreenSize().x + 100, getScreenSize().y + 100 + 50 * i);*/
	}
}


void ArcadeMachine::exitGame()
{
	m_playingGame = false;
}

void ArcadeMachine::events(const sf::Event &event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		if (m_playingGame)
		{
			// Kill switch for all games
			if (event.key.code == sf::Keyboard::Escape)
				exitGame();
		}
		else
		{
			// Exit machine
			if (event.key.code == sf::Keyboard::A)
			{
				ResourceLoader::instance().unloadResourceFile("loadfiles/ResourceLoad_ArcadeMachine.txt");
				m_machine.popState();
			}
			else if (event.key.code == sf::Keyboard::D)
			{
				m_playingGame = true;
				m_games[m_currentGameIndex]->onGameStart();
			}
			else if (event.key.code == sf::Keyboard::W)
				--m_currentGameIndex;
			else if (event.key.code == sf::Keyboard::S)
				++m_currentGameIndex;

			m_currentGameIndex = Utility::clampValue<std::size_t>(m_currentGameIndex, 0, GameCount - 1);
		}

	}

	if (m_playingGame)
		m_games[m_currentGameIndex]->events(event);
}
void ArcadeMachine::update(float deltaTime)
{
	if (m_playingGame)
	{
		m_currentGameText.setString("Playing game: " + m_games[m_currentGameIndex]->getName());
		m_games[m_currentGameIndex]->update(deltaTime);
	}
	else
	{
		m_currentGameText.setString("");
		for (std::size_t i = 0; i < GameCount; i++)
		{
			sf::Sprite &gameButton = m_gameSelectionButtons[i];

			if (i == m_currentGameIndex)
				gameButton.setColor(sf::Color::Green);
			else
				gameButton.setColor(sf::Color::White);
		}
	}
}
void ArcadeMachine::draw()
{
	if (m_playingGame)
	{
		m_games[m_currentGameIndex]->draw();
	}
	else
	{

		for (auto &button : m_gameSelectionButtons)
			Renderer::instance().drawHUD(button);

		/*for (auto &name : m_gameNames)
			Renderer::instance().drawHUD(name);*/
	}

	// Screen is drawn above the game, since it's transparent
	Renderer::instance().drawHUD(m_arcadeBackground);
	Renderer::instance().drawHUD(m_currentGameText);
}

sf::Vector2f ArcadeMachine::getScreenPos() const
{
	return sf::Vector2f(m_arcadeBackground.getPosition().x + 290.f, m_arcadeBackground.getPosition().y + 10.f);;
}
sf::Vector2i ArcadeMachine::getScreenSize() const
{
	return sf::Vector2i(m_arcadeBackground.getGlobalBounds().width - (580.f), m_arcadeBackground.getGlobalBounds().height - 20.f);
}