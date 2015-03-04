#include "StartProgramIntro.h"
#include "Notification.h"
#include "StateMachine.h"
#include "OptionsMenu.h"
#include "ResourceLoader.h"
#include "Button.h"
#include "Renderer.h"

StartProgramIntro::StartProgramIntro(StateMachine &machine)
:
State(machine),
m_currentState(IntroStates::MouseInput)
{
	ResourceLoader::instance().loadResourceFile("loadfiles/ResourceLoad_StartProgramIntro.txt");

	m_mouseInputButton = GuiPtr(new Button(
		ResourceLoader::instance().retrieveTexture("NewGame_Normal"),
		ResourceLoader::instance().retrieveTexture("NewGame_Pressed"),
		sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2 - 200)));
}

void StartProgramIntro::events(const sf::Event &event)
{

}
void StartProgramIntro::update(float deltaTime)
{
	Notification::instance().update(deltaTime, m_window);

	if (m_currentState == IntroStates::MouseInput)
	{
		if (m_mouseInputButton->getReleased(m_machine.getWindow()))
		{
			Notification::instance().write("You have validated mouse input, moving onto keyboard input");
			m_currentState = IntroStates::MouseOutro;
		}
	}
	else if (m_currentState == IntroStates::MouseOutro)
	{
		if (!Notification::instance().isShown())
		{
			m_machine.popState();
			m_machine.pushState<OptionsMenu>();
		}
			m_currentState = IntroStates::KeyboardInput;
	}

	/*if (!m_instructions.empty())
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
	}*/

	//if (!Notification::instance().isShown() && m_instructions)
}
void StartProgramIntro::draw()
{
	Notification::instance().draw();

	if (m_currentState == IntroStates::MouseInput)
	{
		m_mouseInputButton->draw();
	}
}

void StartProgramIntro::onStop()
{

}
void StartProgramIntro::onPlay()
{

}
