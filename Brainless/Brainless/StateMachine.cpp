#include "StateMachine.h"
#include "Renderer.h"
#include "State.h"

StateMachine::StateMachine()
:
m_window(sf::VideoMode(1280, 720), "Brainless", sf::Style::Close)
{
	Renderer::instance().setTarget(m_window);
}

void StateMachine::popState()
{
	m_removeRequests.push_back(std::move(m_states.back()));
	m_states.pop_back();
}

void StateMachine::run()
{
	loop();
}

void StateMachine::loop()
{
	sf::Clock tickClock;
	while (m_window.isOpen())
	{

		// Get delta time for time based movement
		float deltaTime = tickClock.restart().asSeconds();

		if (deltaTime >= 0.01f)
			deltaTime = 0.01f;

		// Handle pop requests
		for (std::size_t i = 0; i < m_removeRequests.size(); i++)
			m_removeRequests.pop_back();

		sf::Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_window.close();

			// Only top state gets events
			if (!m_states.empty())
				m_states.back()->events(event);
		}

		// Only top state gets updates
		if (!m_states.empty())
			m_states.back()->update(deltaTime);

		if (m_window.hasFocus())
		{
			m_window.setActive(true);
			m_window.clear(sf::Color::Black);

			// All states are rendered
			for (int i = m_states.size() - 1; i >= 0; i--)
				m_states[i]->draw();

			m_window.display();
		}
		else
			m_window.setActive(false);
	}
}


sf::RenderWindow &StateMachine::getWindow()
{
	return m_window;
}