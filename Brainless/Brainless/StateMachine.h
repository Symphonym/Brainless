#ifndef INCLUDED_STATE_MACHINE_H
#define INCLUDED_STATE_MACHINE_H

#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>
#include "Notification.h"
#include "ConversationBox.h"
#include "Animation.h"

class Game;
class State;
class StateMachine
{
public:

	typedef std::unique_ptr<State> StatePtr;

	StateMachine();

	template<typename TType> State* pushState()
	{
		m_loadingText.clear();
		m_window.setView(m_window.getDefaultView());

		if (!m_states.empty())
			m_states.back()->onStop();
		m_states.push_back(std::move(StatePtr(new TType(*this))));
		m_states.back()->onPlay();
		return m_states.back().get();
	};
	void popState();

	void run();

	sf::RenderWindow &getWindow();

private:

	float m_savedVolume;

	Animation m_loadingAnim;
	Animation m_loadingAnimDot;

	sf::Texture m_loadingBar;
	sf::Sprite m_loadingBG;
	sf::Clock m_loadingClock;
	sf::Sprite m_loadingAnimSprite;
	sf::Sprite m_loadingAnimSpriteDot;
	sf::Sprite m_loadingSprite;
	std::vector<sf::Text> m_loadingText;

	void loop();

	void draw();

	std::vector<StatePtr> m_states;
	std::vector<StatePtr> m_removeRequests;

	sf::RenderWindow m_window;
};

#endif