#include "StateMachine.h"
#include "Renderer.h"
#include "State.h"
#include "Cursor.h"
#include "ResourceLoader.h"

StateMachine::StateMachine()
:
m_window(sf::VideoMode(1280, 720), "Brainless", sf::Style::Close)
{
	Renderer::instance().setTarget(m_window);

	// Create a little loading screen
	ResourceLoader::instance().setLoadingHandler([&](const std::string &info) -> void
	{
		sf::Text newText;
		newText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
		newText.setString(info);
		newText.setCharacterSize(12);
		newText.setColor(sf::Color::Green);
		m_loadingText.push_back(newText);

		m_loadingSprite.rotate(10.f);

		m_window.clear();

		// Draw states as normal
		draw();

		if (m_loadingText.size() > 10)
			m_loadingText.erase(m_loadingText.begin());

		for (std::size_t i = 0; i < m_loadingText.size(); i++)
		{
			m_loadingText[i].setPosition(0, (15.f)*i);
			Renderer::instance().drawHUD(m_loadingText[i]);
		}

		Renderer::instance().drawHUD(m_loadingSprite);
		Renderer::instance().executeDraws();

		m_window.display();
	});

	m_loadingSprite.setTexture(ResourceLoader::instance().retrieveTexture("LoadingCross"));
	m_loadingSprite.setOrigin(
		m_loadingSprite.getGlobalBounds().width / 2,
		m_loadingSprite.getGlobalBounds().height / 2);
	m_loadingSprite.setPosition(
		m_loadingSprite.getGlobalBounds().width / 2,
		m_window.getSize().y - m_loadingSprite.getGlobalBounds().height / 2);

	// Hide mouse cursor
	m_window.setMouseCursorVisible(false);
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

		// Update cursor regardless of state
		Cursor::instance().update(m_window);

		if (m_window.hasFocus())
		{
			m_window.setActive(true);
			m_window.clear();
			draw();
			m_window.display();
		}
		else
			m_window.setActive(false);
	}
}


void StateMachine::draw()
{
	// All states are rendered
	for (int i = m_states.size() - 1; i >= 0; i--)
		m_states[i]->draw();

	// Draw cursor
	Cursor::instance().draw();

	Renderer::instance().executeDraws();
}

sf::RenderWindow &StateMachine::getWindow()
{
	return m_window;
}