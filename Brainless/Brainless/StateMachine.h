#ifndef INCLUDED_STATE_MACHINE_H
#define INCLUDED_STATE_MACHINE_H

#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>

class Game;
class State;
class StateMachine
{
public:

	typedef std::unique_ptr<State> StatePtr;

	StateMachine();

	template<typename TType> void pushState()
	{
		m_window.setView(m_window.getDefaultView());
		m_states.push_back(std::move(StatePtr(new TType(*this))));
	};
	void popState();

	void run();

	sf::RenderWindow &getWindow();

private:

	void loop();

	std::vector<StatePtr> m_states;
	std::vector<StatePtr> m_removeRequests;

	sf::RenderWindow m_window;
};

#endif