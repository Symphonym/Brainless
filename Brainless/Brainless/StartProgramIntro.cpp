#include "StartProgramIntro.h"
#include "Notification.h"
#include "StateMachine.h"
#include "MainMenu.h"
#include "ResourceLoader.h"

StartProgramIntro::StartProgramIntro(StateMachine &machine)
:
State(machine)
{
	ResourceLoader::instance().loadResourceFile("loadfiles/ResourceLoad_NewGameIntro.txt");
	m_instructions.push_back(std::make_pair(
		"Press the left mouse button to advance", []() -> bool
	{
		return sf::Mouse::isButtonPressed(sf::Mouse::Left);
	}));
	m_instructions.push_back(std::make_pair(
		"Press the B button to advance", []() -> bool
	{
		return sf::Keyboard::isKeyPressed(sf::Keyboard::B);
	}));

	Notification::instance().write(m_instructions.back().first);
}

void StartProgramIntro::events(const sf::Event &event)
{

}
void StartProgramIntro::update(float deltaTime)
{
	Notification::instance().update(deltaTime, m_window);

	if (!m_instructions.empty())
	{
		// Instruction action was completed
		if (m_instructions.back().second())
		{
			// Last action was completed
			if (m_instructions.size() == 1)
			{
				m_machine.popState();
				m_machine.pushState<MainMenu>();
			}
			else
			{
				m_instructions.pop_back();
				Notification::instance().write(m_instructions.back().first);
			}

		}
	}
	else
	{
		m_machine.popState();
		m_machine.pushState<MainMenu>();
	}

	//if (!Notification::instance().isShown() && m_instructions)
}
void StartProgramIntro::draw()
{
	Notification::instance().draw();
}

void StartProgramIntro::onStop()
{

}
void StartProgramIntro::onPlay()
{

}


void StartProgramIntro::pumpInstruction()
{

}