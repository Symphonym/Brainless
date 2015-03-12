#include "PauseMenu.h"
#include "ResourceLoader.h"
#include "Button.h"
#include "MainMenu.h"
#include "StateMachine.h"
#include "Renderer.h"
#include "Game.h"

PauseMenu::PauseMenu(StateMachine &machine)
:
State(machine),
m_game(nullptr)
{
	m_buttons[0] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("Resume_Normal"),
		ResourceLoader::instance().retrieveTexture("Resume_Normal"),
		sf::Vector2f(m_window.getSize().x / 2 + 40.f, m_window.getSize().y / 2 - 200.f)));

	m_buttons[1] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("saveGamePM_Normal"),
		ResourceLoader::instance().retrieveTexture("saveGamePM_Normal"),
		sf::Vector2f(m_window.getSize().x / 2 + 40.f, m_window.getSize().y / 2 - 100.f))); //160

	m_buttons[2] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("MainMenuBack_Normal"),
		ResourceLoader::instance().retrieveTexture("MainMenuBack_Normal"),
		sf::Vector2f(m_window.getSize().x / 2 + 40.f, m_window.getSize().y / 2 - 0.f))); //90


	/*m_buttons[2] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("saveGamePM_Normal"),
		ResourceLoader::instance().retrieveTexture("saveGamePM_Normal"),
		sf::Vector2f(m_window.getSize().x / 2 + 40.f, m_window.getSize().y / 2 - 90.f)));

	m_buttons[3] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("loadGamePM_Normal"),
		ResourceLoader::instance().retrieveTexture("loadGamePM_Normal"),
		sf::Vector2f(m_window.getSize().x / 2 + 40.f, m_window.getSize().y / 2 - 20.f)));

	m_buttons[4] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("optionsPM_Normal"),
		ResourceLoader::instance().retrieveTexture("optionsPM_Normal"),
		sf::Vector2f(m_window.getSize().x / 2 + 40.f, m_window.getSize().y / 2 + 50.f)));

	m_buttons[5] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("quitPM_Normal"),
		ResourceLoader::instance().retrieveTexture("quitPM_Normal"),
		sf::Vector2f(m_window.getSize().x / 2 + 40.f, m_window.getSize().y / 2 + 120.f)));*/

	m_background.setTexture(ResourceLoader::instance().retrieveTexture("PauseBackground"));
}

void PauseMenu::giveGame(Game &game)
{
	m_game = &game;
}

void PauseMenu::update(float deltaTime)
{
	// Resume
	if (m_buttons[0]->getReleased(m_machine.getWindow()))
		m_machine.popState();

	// Save game
	else if (m_buttons[1]->getReleased(m_machine.getWindow()))
	{
		if (m_game)
			m_game->saveGame();
	}

	// Back to mainmenu
	else if (m_buttons[2]->getReleased(m_machine.getWindow()))
	{
		m_machine.popState(); // Pop pause
		m_machine.popState(); // Pop game
		m_machine.pushState<MainMenu>();
	}
}
void PauseMenu::draw()
{
	Renderer::instance().drawHUD(m_background);

	for (auto &button : m_buttons)
		button->draw();
}