#include "ArcadeMachine.h"
#include "Renderer.h"
#include "StateMachine.h"
#include "Utility.h"
#include "ArcadeGame.h"
#include "ResourceLoader.h"

ArcadeMachine::ArcadeMachine(StateMachine &machine)
:
State(machine),
m_currentGameIndex(0),
m_playingGame(false)
{
	// Load game resources
	ResourceLoader::instance().loadResourceFile("loadfiles/ResourceLoad_ArcadeMachine.txt");

	m_arcadeBackground.setTexture(ResourceLoader::instance().retrieveTexture("ArcadeMachineScreen"));
	m_arcadeBackground.setPosition(
		m_window.getSize().x / 2.f - m_arcadeBackground.getGlobalBounds().width / 2.f,
		0);

	// ADD YOUR GAMES HERE
	//m_games[0] = GamePtr(new ...);
	//m_games[1] = GamePtr(new ...);
	//m_games[2] = GamePtr(new ...);
	//m_games[3] = GamePtr(new ...);
	//m_games[4] = GamePtr(new ...);

	m_currentGameText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_currentGameText.setPosition(getScreenPos());

	for (std::size_t i = 0; i < m_gameSelectionButtons.size(); i++)
	{
		sf::Sprite &button = m_gameSelectionButtons[i];
		button.setTexture(ResourceLoader::instance().retrieveTexture("ArcadeMenuButton"));
		button.setPosition(
			getScreenPos().x + getScreenSize().x / 2.f - button.getGlobalBounds().width / 2.f,
			getScreenPos().y + 100 + (button.getGlobalBounds().height + 5.f)*i);
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

			m_games[m_currentGameIndex]->events(event);

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
				m_playingGame = true;
			else if (event.key.code == sf::Keyboard::W)
				--m_currentGameIndex;
			else if (event.key.code == sf::Keyboard::S)
				++m_currentGameIndex;

			m_currentGameIndex = Utility::clampValue<std::size_t>(m_currentGameIndex, 0, GameCount - 1);
		}

	}
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
		for (std::size_t i = 0; i < m_gameSelectionButtons.size(); i++)
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

		// Screen is drawn above the game, since it's transparent
		Renderer::instance().drawHUD(m_arcadeBackground);
	}
}

sf::Vector2f ArcadeMachine::getScreenPos() const
{
	return m_arcadeBackground.getPosition();
}
sf::Vector2i ArcadeMachine::getScreenSize() const
{
	return sf::Vector2i(m_arcadeBackground.getGlobalBounds().width, m_arcadeBackground.getGlobalBounds().height);
}