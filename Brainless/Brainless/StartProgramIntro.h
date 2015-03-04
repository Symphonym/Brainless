#ifndef INCLUDED_START_PROGRAM_INTRO_H
#define INCLUDED_START_PROGRAM_INTRO_H

#include "State.h"
#include <vector>
#include <functional>
#include <memory>
#include "GUIElement.h"

class StartProgramIntro : public State
{
public:

	StartProgramIntro(StateMachine &machine);

	virtual void events(const sf::Event &event);
	virtual void update(float deltaTime);
	virtual void draw();

	virtual void onStop();
	virtual void onPlay();

private:

	enum class IntroStates
	{
		MouseInput,
		MouseOutro,
		KeyboardInput,
		KeyboardOutro
	};
	IntroStates m_currentState;

	typedef std::unique_ptr<GUIElement> GuiPtr;
	GuiPtr m_mouseInputButton;
	sf::Sprite m_mouseInputBG;

	struct KeyboardInput
	{
		sf::Text text;
		sf::Keyboard::Key inputKey;
		bool validated;
	};
	std::vector<KeyboardInput> m_keyboardInputs;
	sf::Sprite m_keyInputBG;
	void createKeyboardInput(const std::string &displayText, sf::Keyboard::Key inputKey);

};

#endif