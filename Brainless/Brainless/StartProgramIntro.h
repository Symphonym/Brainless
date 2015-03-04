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

	// Pumps the next instruction into the system
	void pumpInstruction();

	typedef std::function<bool()> InstructionFunc;

	// Text to display for instruction, and and function containing info about what actions must be done
	// to move onto the next action
	typedef std::pair <std::string, InstructionFunc> Instruction;

	std::vector<Instruction> m_instructions;
};

#endif