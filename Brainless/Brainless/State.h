#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include <SFML\Graphics.hpp>

class StateMachine;
class State
{
public:

	State(StateMachine &machine);
	virtual ~State();

	virtual void events(const sf::Event &event) {};
	virtual void update(float deltaTime) {};
	virtual void draw() {};

	// Called when the state is removed or no longer on the top of the stack
	virtual void onStop() {};
	// Called when the state reaches the top of the stack
	virtual void onPlay() {};

	const sf::RenderWindow& getWindow() const;
	StateMachine& getMachine();

protected:

	friend class StateMachine;

	StateMachine &m_machine;
	sf::RenderWindow &m_window;

};

#endif