#include "State.h"
#include "StateMachine.h"

State::State(StateMachine &machine)
:
m_machine(machine),
m_window(machine.getWindow())
{

}

const sf::RenderWindow &State::getWindow() const
{
	return m_machine.getWindow();
}

StateMachine& State::getMachine()
{
	return m_machine;
}