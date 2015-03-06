#include "StateMachine.h"
#include "Renderer.h"
#include "State.h"
#include "Cursor.h"
#include "ResourceLoader.h"

StateMachine::StateMachine()
:
m_window(sf::VideoMode(1280, 720), "Brainless", sf::Style::Close),
m_loadingAnim(256, 256),
m_loadingAnimDot(256, 256)
{
	Renderer::instance().setTarget(m_window);


	// Create a little loading screen
	ResourceLoader::instance().setLoadingHandler([&](const std::string &info, int current, int total) -> void
	{
		float deltaTime = m_loadingClock.restart().asSeconds();
		if (deltaTime > 1)
			deltaTime = 0;

		sf::Text newText;
		newText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
		newText.setString(info);
		newText.setCharacterSize(20);
		newText.setColor(sf::Color::Color(0,255,0,150));
		m_loadingText.push_back(newText);

		m_window.clear();

		float completeness = static_cast<float>(current) / static_cast<float>(total);
		m_loadingSprite.setScale(completeness*m_window.getSize().x, 1);

		int r = 255 + (0 - 255) * completeness;
		int g = 0 + (255 - 0) * completeness;
		m_loadingSprite.setColor(sf::Color::Color(
			r,
			g,
			0));

		//if (std::rand() % 2 == 0)
		//	sf::sleep(sf::milliseconds(50));

		// Draw states as normal
		draw();

		Renderer::instance().drawHUD(m_loadingBG);

		if (m_loadingText.size() > 10)
			m_loadingText.erase(m_loadingText.begin());

		/*for (std::size_t i = 0; i < m_loadingText.size(); i++)
		{
			m_loadingText[i].setPosition(0, (15.f)*i);
			Renderer::instance().drawHUD(m_loadingText[i]);
		}*/

		m_loadingAnimSprite.setTextureRect(m_loadingAnim.getRectangle(deltaTime));
		m_loadingAnimSpriteDot.setTextureRect(m_loadingAnimDot.getRectangle(deltaTime));
		Renderer::instance().drawHUD(m_loadingSprite);
		Renderer::instance().drawHUD(m_loadingAnimSprite);
		Renderer::instance().drawHUD(m_loadingAnimSpriteDot);
		Renderer::instance().executeDraws();

		m_window.display();
	});

	m_loadingBG.setTexture(ResourceLoader::instance().retrieveTexture("LoadingScreen_BG"));
	m_loadingAnimSprite.setTexture(ResourceLoader::instance().retrieveTexture("LoadingScreen_Run"));
	m_loadingAnimSpriteDot.setTexture(ResourceLoader::instance().retrieveTexture("LoadingScreen_Dot"));
	m_loadingAnimSprite.setPosition(
		m_window.getSize().x / 2 - 150.f,
		m_window.getSize().y - 400.f);
	m_loadingAnimSpriteDot.setPosition(
		m_window.getSize().x / 2 + 155.f,
		m_window.getSize().y - 170.f);

	m_loadingAnim.loop(0, 7, 0, 5.f);
	m_loadingAnimDot.loop(0, 2, 0, 2.f);

	sf::Image loadingBar;
	loadingBar.create(1, 7, sf::Color::White);

	m_loadingBar.loadFromImage(loadingBar);
	m_loadingSprite.setTexture(m_loadingBar);

	m_loadingSprite.setPosition(
		0,
		m_window.getSize().y - m_loadingSprite.getGlobalBounds().height);


	// Hide mouse cursor
	m_window.setMouseCursorVisible(false);
}

void StateMachine::popState()
{
	m_removeRequests.push_back(std::move(m_states.back()));
	m_removeRequests.back()->onStop();
	m_states.pop_back();

	if (!m_states.empty())
		m_states.back()->onPlay();
}

void StateMachine::run()
{
	loop();
}

void StateMachine::loop()
{
	sf::Clock tickClock;

	int frameLimit = 0; // 0 om avstängd

	while (m_window.isOpen())
	{

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
		{
			frameLimit = 0;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
		{
			frameLimit = 60;
		}

		m_window.setFramerateLimit(frameLimit); //för kollision test vid låg fps
		// Get delta time for time based movement
		float deltaTime = tickClock.restart().asSeconds();
	
		if (deltaTime >= 0.0167f)
			deltaTime = 0.0167f;
	
		// Handle pop requests
		for (std::size_t i = 0; i < m_removeRequests.size(); i++)
			m_removeRequests.pop_back();

		State* thisState = m_states.empty() ? nullptr : m_states.back().get();

		sf::Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_window.close();

			// Only top state gets events
			if (!m_states.empty())
				thisState->events(event);
		}

		// Only top state gets updates
		if (!m_states.empty())
			thisState->update(deltaTime);

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
	//for (int i = m_states.size() - 1; i >= 0; i--)
	//	m_states[i]->draw();
	for (std::size_t i = 0; i < m_states.size(); i++)
	{
		// Draw each state individually
		m_states[i]->draw();
	//if (m_state)
	//m_states.back()->draw();
		Renderer::instance().executeDraws();
	}

	// Draw cursor
	Cursor::instance().draw();
	Renderer::instance().executeDraws();
}

sf::RenderWindow &StateMachine::getWindow()
{
	return m_window;
}