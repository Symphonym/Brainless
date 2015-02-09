#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include <SFML\Graphics.hpp>

class StateMachine;
class State
{
public:

	State(StateMachine &machine);

	virtual void events(const sf::Event &event) {};
	virtual void update(float deltaTime) {};
	virtual void draw() {};

	const sf::RenderWindow &getWindow() const;

protected:

	friend class StateMachine;

	StateMachine &m_machine;
	sf::RenderWindow &m_window;

};

#endif