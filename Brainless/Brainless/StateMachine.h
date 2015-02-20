#ifndef INCLUDED_STATE_MACHINE_H
#define INCLUDED_STATE_MACHINE_H

#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>
#include "Notification.h"
#include "ConversationBox.h"

class Game;
class State;
class StateMachine
{
public:

	typedef std::unique_ptr<State> StatePtr;

	StateMachine();

	template<typename TType> void pushState()
	{
		m_loadingText.clear();
		m_window.setView(m_window.getDefaultView());
		m_states.push_back(std::move(StatePtr(new TType(*this))));
		// TODO DISABLE NOTIFICATION AND CONVO BOX Notification::instance().s
	};
	void popState();

	void run();

	sf::RenderWindow &getWindow();

private:

	sf::Texture m_loadingBar;
	sf::Sprite m_loadingSprite;
	std::vector<sf::Text> m_loadingText;

	void loop();

	void draw();

	std::vector<StatePtr> m_states;
	std::vector<StatePtr> m_removeRequests;

	sf::RenderWindow m_window;
};

#endif