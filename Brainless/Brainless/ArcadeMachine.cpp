#include "ArcadeMachine.h"
#include "Renderer.h"
#include "StateMachine.h"
#include "Utility.h"
#include "ArcadeGame.h"
#include "ResourceLoader.h"
#include "BeachParty.h"
#include "Boop.h"

#include "NoteGame.h"
#include "RobotAkeAttack.h"
#include "RpgGame.h"

#include "SoundPlayer.h"
#include "OptionsMenu.h"

ArcadeMachine::ArcadeMachine(StateMachine &machine)
:
State(machine),
m_currentGameIndex(0),
m_playingGame(false)
{
	// Load game resources
	ResourceLoader::instance().loadResourceFile("loadfiles/ResourceLoad_ArcadeMachine.txt");

	m_arcadeForeground.setTexture(ResourceLoader::instance().retrieveTexture("ArcadeMachineScreen"));
	m_arcadeBackground.setTexture(ResourceLoader::instance().retrieveTexture("ArcadeBG"));
	m_arcadeBackground.setPosition(sf::Vector2f(m_arcadeForeground.getPosition().x + 290.f, m_arcadeForeground.getPosition().y + 10.f));

	// ADD YOUR GAMES HERE
	//m_games[0] = GamePtr(new BeachParty(*this));
	//m_games[1] = GamePtr(new NoteGame(*this));
	//m_games[2] = GamePtr(new RobotAkeAttack(*this));
	//m_games[3] = GamePtr(new RpgGame(*this));
	//m_games[2] = GamePtr(new ...);
	//m_games[3] = GamePtr(new ...);

	//m_games[4] = GamePtr(new Boop(*this));
	//m_games[4] = GamePtr(new ...);

	m_currentGameText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_currentGameText.setPosition(getScreenPos());
	m_currentGameText.setColor(sf::Color(0, 120, 0, 255));
	m_infoText = m_currentGameText;
	m_infoText.setPosition(300, 550);
	m_infoText.setString("");
	m_infoText.setString(m_infoText.getString() + OptionsMenu::getKeybindKeyName("Up") + " = UP" + '\n');
	m_infoText.setString(m_infoText.getString() + OptionsMenu::getKeybindKeyName("Down") + " = DOWN" + '\n');
	m_infoText.setString(m_infoText.getString() + OptionsMenu::getKeybindKeyName("Right") + " = PLAY" + '\n');
	m_infoText.setString(m_infoText.getString() + OptionsMenu::getKeybindKeyName("Left") + " = EXIT");
	m_escText = m_infoText;
	m_escText.setPosition(getScreenPos());
	m_escText.setString("ESC = EXIT");


	m_noGameText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_noGameText.setString("No games are loaded");
	m_noGameText.setPosition(
		getScreenPos().x + getScreenSize().x / 2.f - m_noGameText.getGlobalBounds().width / 2.f,
		getScreenPos().y + getScreenSize().y / 2.f - m_noGameText.getGlobalBounds().height / 2.f);
	m_noGameText.setColor(sf::Color(0, 120, 0, 255));

	/*for (std::size_t i = 0; i < m_gameSelectionButtons.size(); i++)
	{
		sf::Text &button = m_gameSelectionButtons[i];
		button.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
		button.setCharacterSize(50);
		if (m_games[i])
			button.setString(m_games[i]->getName());
		else
			button.setString("No game in slot");
		button.setPosition(
			getScreenPos().x + getScreenSize().x / 2.f - button.getGlobalBounds().width / 2.f,
			getScreenPos().y + 100 + (50.f)*i);


	}*/

	SoundPlayer::instance().playMusic("ArcadeMusic", true, 20);
}

void ArcadeMachine::loadArcadeGame(ArcadeGame *game)
{
	sf::Text button;
	button.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	button.setCharacterSize(50);
	button.setString(game->getName());

	m_gameSelectionButtons.push_back(button);
	m_games.push_back(std::move(GamePtr(game)));

	refreshMenu();
}

void ArcadeMachine::refreshMenu()
{
	for (std::size_t i = 0; i < m_gameSelectionButtons.size(); i++)
	{
		sf::Text &button = m_gameSelectionButtons[i];
		button.setPosition(
			getScreenPos().x + getScreenSize().x / 2.f - button.getGlobalBounds().width / 2.f,
			getScreenPos().y + 100 + (50.f)*i);
	}
}

void ArcadeMachine::onPlay()
{
	SoundPlayer::instance().playMusic("ArcadeMusic", true, 20);
}
void ArcadeMachine::onStop()
{
	SoundPlayer::instance().stopMusic("ArcadeMusic");
}

void ArcadeMachine::exitGame()
{
	m_playingGame = false;

	if (ResourceLoader::instance().retrieveMusic("ArcadeMusic").getStatus() != sf::Music::Playing)
		SoundPlayer::instance().playMusic("ArcadeMusic", true, 20);
}

void ArcadeMachine::events(const sf::Event &event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		if (m_playingGame)
		{
			// Kill switch for all games
			if (event.key.code == sf::Keyboard::Escape)
			{
				exitGame();
				SoundPlayer::instance().playSound("ArcadeDark", getScreenPos(), 10);
			}
		}
		else
		{
			// Exit machine
			if (event.key.code == OptionsMenu::getKeybind("Left"))
			{
				SoundPlayer::instance().killAllSounds();
				ResourceLoader::instance().unloadResourceFile("loadfiles/ResourceLoad_ArcadeMachine.txt", false);
				m_machine.popState();
			}
			if (!m_games.empty())
			{
				if (event.key.code == OptionsMenu::getKeybind("Right"))
				{
					if (m_games[m_currentGameIndex])
					{
						m_playingGame = true;
						m_games[m_currentGameIndex]->onGameStart();
						SoundPlayer::instance().playSound("ArcadeLight", getScreenPos(), 10);
					}
				}
				else if (event.key.code == OptionsMenu::getKeybind("Up"))
				{
					--m_currentGameIndex;
					SoundPlayer::instance().playSound("ArcadeMedium", getScreenPos(), 10);
				}
				else if (event.key.code == OptionsMenu::getKeybind("Down"))
				{
					++m_currentGameIndex;
					SoundPlayer::instance().playSound("ArcadeMedium", getScreenPos(), 10);
				}

				m_currentGameIndex = Utility::clampValue<std::size_t>(m_currentGameIndex, 0, m_games.size() - 1);
			}

			
		}

	}

	if (m_playingGame)
		m_games[m_currentGameIndex]->events(event);
}
void ArcadeMachine::update(float deltaTime)
{
	SoundPlayer::instance().update(deltaTime, getScreenPos());

	if (m_games.empty())
		return;

	if (m_playingGame)
	{
		m_currentGameText.setString("Playing game: " + m_games[m_currentGameIndex]->getName());
		m_games[m_currentGameIndex]->update(deltaTime);
	}
	else
	{
		m_currentGameText.setString("");
		for (std::size_t i = 0; i < m_games.size(); i++)
		{
			sf::Text &gameButton = m_gameSelectionButtons[i];

			if (i == m_currentGameIndex)
				gameButton.setColor(sf::Color::Green);
			else
				gameButton.setColor(sf::Color(0, 120, 0, 255));
		}
	}
}
void ArcadeMachine::draw()
{
	Renderer::instance().drawHUD(m_arcadeBackground);

	if (m_games.empty())
		Renderer::instance().drawHUD(m_noGameText);

	if (m_playingGame && !m_games.empty())
	{
		m_games[m_currentGameIndex]->draw();
		Renderer::instance().drawHUD(m_escText);
	}
	else
	{

		for (auto &button : m_gameSelectionButtons)
			Renderer::instance().drawHUD(button);

		Renderer::instance().drawHUD(m_infoText);
	}

	// Screen is drawn above the game, since it's transparent
	Renderer::instance().drawHUD(m_arcadeForeground);
	//Renderer::instance().drawHUD(m_currentGameText);
}

sf::Vector2f ArcadeMachine::getScreenPos() const
{
	return sf::Vector2f(m_arcadeForeground.getPosition().x + 290.f, m_arcadeForeground.getPosition().y + 10.f);;
}
sf::Vector2i ArcadeMachine::getScreenSize() const
{
	return sf::Vector2i(m_arcadeForeground.getGlobalBounds().width - (580.f), m_arcadeForeground.getGlobalBounds().height - 20.f);
}
