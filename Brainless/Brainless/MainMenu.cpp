#include "MainMenu.h"
#include "StateMachine.h"
#include "Game.h"
#include "Button.h"
#include "ResourceLoader.h"
#include "SoundPlayer.h"
#include "Renderer.h"
#include "FileSave.h"

MainMenu::MainMenu(StateMachine &machine)
:
State(machine),
m_newGameConfirmWindow(false)
{
	ResourceLoader::instance().loadResourceFile("loadfiles/ResourceLoad_MainMenu.txt");

	m_buttons[0] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("NewGame_Normal"),
		ResourceLoader::instance().retrieveTexture("NewGame_Pressed"),
		sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2)));

	m_buttons[1] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("LoadGame_Normal"),
		ResourceLoader::instance().retrieveTexture("LoadGame_Pressed"),
		sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2 + 100.f)));


	// New game confirm data
	m_confirmBackground.setTexture(ResourceLoader::instance().retrieveTexture("NewGameConfirmBG"));
	m_confirmBackground.setPosition(
		m_window.getSize().x / 2, m_window.getSize().y / 2);

	m_newGameButtons[0] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("Yes_Normal"),
		ResourceLoader::instance().retrieveTexture("Yes_Pressed"),
		sf::Vector2f(m_confirmBackground.getPosition().x + 20.f, m_confirmBackground.getPosition().y + 120.f)));

	m_newGameButtons[1] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("No_Normal"),
		ResourceLoader::instance().retrieveTexture("No_Pressed"),
		sf::Vector2f(m_confirmBackground.getPosition().x + 210.f, m_confirmBackground.getPosition().y + 120.f)));

	//Play music
	SoundPlayer::instance().playMusic("MenuMusic",true,20);
}

void MainMenu::update(float deltaTime)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(m_machine.getWindow());

	if (!m_newGameConfirmWindow)
	{

		// New game
		if (m_buttons[0]->getReleased(mousePos))
			m_newGameConfirmWindow = true;

		// Load game
		else if (m_buttons[1]->getReleased(mousePos))
		{
			m_machine.popState();
			m_machine.pushState<Game>();
		}
	}
	else
	{
		// Yes
		if (m_newGameButtons[0]->getReleased(mousePos))
		{
			FileSave::wipeProgress();
			m_machine.popState();
			m_machine.pushState<Game>();
		}

		// No
		else if (m_newGameButtons[1]->getReleased(mousePos))
			m_newGameConfirmWindow = false;
	}
}
void MainMenu::draw()
{
	for (auto &button : m_buttons)
		button->draw();

	if (m_newGameConfirmWindow)
	{
		Renderer::instance().drawHUD(m_confirmBackground);

		for (auto &button : m_newGameButtons)
			button->draw();
	}

	Renderer::instance().executeDraws();
}