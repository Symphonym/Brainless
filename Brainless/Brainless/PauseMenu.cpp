#include "PauseMenu.h"
#include "ResourceLoader.h"
#include "Button.h"
#include "MainMenu.h"
#include "StateMachine.h"
#include "Renderer.h"

PauseMenu::PauseMenu(StateMachine &machine)
:
State(machine)
{
	m_buttons[0] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("Resume_Normal"),
		ResourceLoader::instance().retrieveTexture("Resume_Pressed"),
		sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2)));

	m_buttons[1] = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("MainMenuBack_Normal"),
		ResourceLoader::instance().retrieveTexture("MainMenuBack_Pressed"),
		sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2 + 100.f)));

	sf::Image bgImage;
	bgImage.create(m_window.getSize().x, m_window.getSize().y, sf::Color::Color(0,0,0,100));

	m_backgroundTexture.loadFromImage(bgImage);
	m_background.setTexture(m_backgroundTexture);
}

void PauseMenu::update(float deltaTime)
{
	// Resume
	if (m_buttons[0]->getReleased(m_machine.getWindow()))
		m_machine.popState();

	// Back to mainmenu
	else if (m_buttons[1]->getReleased(m_machine.getWindow()))
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