#include "StartProgramIntro.h"
#include "Notification.h"
#include "StateMachine.h"
#include "MainMenu.h"
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
		ResourceLoader::instance().retrieveTexture("ok_Normal"),
		ResourceLoader::instance().retrieveTexture("ok_Pressed"),
		sf::Vector2f(
			m_window.getSize().x / 2 - 30, m_window.getSize().y / 2.f)));

	m_mouseInputBG.setTexture(ResourceLoader::instance().retrieveTexture("StartProgramIntroMouseBG"));
	m_mouseInputBG.setPosition(
		m_window.getSize().x / 2.f - m_mouseInputBG.getGlobalBounds().width / 2.f,
		m_window.getSize().y / 2.f - m_mouseInputBG.getGlobalBounds().height / 2.f);

	m_keyInputBG.setTexture(ResourceLoader::instance().retrieveTexture("StartProgramIntroKeyBG"));
	m_keyInputBG.setPosition(
		m_window.getSize().x / 2.f - m_mouseInputBG.getGlobalBounds().width / 2.f,
		m_window.getSize().y / 2.f - m_mouseInputBG.getGlobalBounds().height / 2.f - 200.f);

	createKeyboardInput("Up: W", sf::Keyboard::W);
	createKeyboardInput("Down: S", sf::Keyboard::S);
	createKeyboardInput("Left: A", sf::Keyboard::A);
	createKeyboardInput("Right: D", sf::Keyboard::D);
	createKeyboardInput("Inventory: I", sf::Keyboard::I);
	createKeyboardInput("Jump: Space", sf::Keyboard::Space);
	createKeyboardInput("Run: Left shift", sf::Keyboard::LShift);
}

void StartProgramIntro::events(const sf::Event &event)
{

}
void StartProgramIntro::update(float deltaTime)
{
	Notification::instance().update(deltaTime, m_window);

	if (m_currentState == IntroStates::MouseInput)
	{
		// Move onto mouse outro state
		if (m_mouseInputButton->getReleased(m_machine.getWindow()))
		{
			Notification::instance().write("You have validated mouse input, moving onto keyboard input");
			m_currentState = IntroStates::MouseOutro;
		}
	}
	else if (m_currentState == IntroStates::MouseOutro)
	{
		// Move onto keyboard input state
		if (!Notification::instance().isShown())
			m_currentState = IntroStates::KeyboardInput;
	}


	else if (m_currentState == IntroStates::KeyboardInput)
	{
		// Validate keyboard input
		bool inputValidated = true;
		for (std::size_t i = 0; i < m_keyboardInputs.size(); i++)
		{
			KeyboardInput &input = m_keyboardInputs[i];

			if (sf::Keyboard::isKeyPressed(input.inputKey))
			{
				input.text.setColor(sf::Color::Green);
				input.validated = true;
			}

			if (!input.validated)
				inputValidated = false;
		}

		// Move onto keyboard outro state
		if (inputValidated)
		{
			Notification::instance().write("All input is validated, moving onto options menu");
			m_currentState = IntroStates::KeyboardOutro;
		}
	}
	else if (m_currentState == IntroStates::KeyboardOutro)
	{
		// Move to game menu
		if (!Notification::instance().isShown())
		{
			m_machine.popState();
			m_machine.pushState<MainMenu>();
		}
	}
}
void StartProgramIntro::draw()
{
	Notification::instance().draw();

	if (m_currentState == IntroStates::MouseInput)
	{
		Renderer::instance().drawHUD(m_mouseInputBG);
		m_mouseInputButton->draw();
	}
	else if (m_currentState == IntroStates::KeyboardInput)
	{
		Renderer::instance().drawHUD(m_keyInputBG);

		for (std::size_t i = 0; i < m_keyboardInputs.size(); i++)
			Renderer::instance().drawHUD(m_keyboardInputs[i].text);
	}
}

void StartProgramIntro::onStop()
{

}
void StartProgramIntro::onPlay()
{

}


void StartProgramIntro::createKeyboardInput(const std::string &displayText, sf::Keyboard::Key inputKey)
{
	KeyboardInput input;
	input.text.setString(displayText);
	input.text.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	input.text.setCharacterSize(55);
	input.text.setPosition(
		m_window.getSize().x / 2.f - 100.f,
		m_window.getSize().y - 2.f - 500.f + m_keyboardInputs.size() * 30.f);
	input.text.setColor(sf::Color::Red);
	input.inputKey = inputKey;
	input.validated = false;
	m_keyboardInputs.push_back(input);
}