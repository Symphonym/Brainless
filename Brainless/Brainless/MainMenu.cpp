#include "MainMenu.h"
#include "StateMachine.h"
#include "Game.h"
#include "Button.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "FileSave.h"

MainMenu::MainMenu(StateMachine &machine)
:
State(machine),
m_newGameConfirmWindow(false)
{
	ResourceLoader::instance().loadFromFile("loadfiles/ResourceLoad_MainMenu.txt");

	m_buttons[0] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("NewGame_Normal"),
		ResourceLoader::instance().retrieveTexture("NewGame_Pressed"),
		sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2),
		&m_window));

	m_buttons[1] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("LoadGame_Normal"),
		ResourceLoader::instance().retrieveTexture("LoadGame_Pressed"),
		sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2 + 100.f),
		&m_window));


	// New game confirm data
	m_confirmBackground.setTexture(ResourceLoader::instance().retrieveTexture("NewGameConfirmBG"));
	m_confirmBackground.setPosition(
		m_window.getSize().x / 2, m_window.getSize().y / 2);

	m_newGameButtons[0] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("Yes_Normal"),
		ResourceLoader::instance().retrieveTexture("Yes_Pressed"),
		sf::Vector2f(m_confirmBackground.getPosition().x + 20.f, m_confirmBackground.getPosition().y + 120.f),
		&m_window));

	m_newGameButtons[1] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("No_Normal"),
		ResourceLoader::instance().retrieveTexture("No_Pressed"),
		sf::Vector2f(m_confirmBackground.getPosition().x + 210.f, m_confirmBackground.getPosition().y + 120.f),
		&m_window));

	//Play music
	ResourceLoader::instance().retrieveMusic("MenuMusic").setLoop(true);
	ResourceLoader::instance().retrieveMusic("MenuMusic").play();
}

void MainMenu::update(float deltaTime)
{
	if (!m_newGameConfirmWindow)
	{
		// New game
		if (m_buttons[0]->getReleased())
			m_newGameConfirmWindow = true;

		// Load game
		else if (m_buttons[1]->getReleased())
		{
			m_machine.popState();
			m_machine.pushState<Game>();
			ResourceLoader::instance().retrieveMusic("MenuMusic").stop();
		}
	}
	else
	{
		// Yes
		if (m_newGameButtons[0]->getReleased())
		{
			FileSave::wipeProgress();
			m_machine.popState();
			m_machine.pushState<Game>();
			ResourceLoader::instance().retrieveMusic("MenuMusic").stop();
		}

		// No
		else if (m_newGameButtons[1]->getReleased())
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