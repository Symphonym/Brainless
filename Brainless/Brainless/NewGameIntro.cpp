#include "NewGameIntro.h"
#include "Notification.h"
#include "Game.h"
#include "StateMachine.h"
#include "ResourceLoader.h"
#include "Renderer.h"

NewGameIntro::NewGameIntro(StateMachine &machine)
:
State(machine),
m_currentIndex(0),
m_curFade(0),
m_maxFade(1)
{
	ResourceLoader::instance().loadResourceFile("loadfiles/ResourceLoad_NewGameIntro.txt");
	//Notification::instance().write("The game has zombies and shit, pretty crazy");

	pushIntroText("LORE");
	pushIntroText("MORE LORE");
}

void NewGameIntro::events(const sf::Event &event)
{

}
void NewGameIntro::update(float deltaTime)
{
	Notification::instance().update(deltaTime, m_window);
	if (!m_introTexts.empty())
	{
		sf::Text &currentText = m_introTexts[m_currentIndex];

		float fadePercent = m_curFade / m_maxFade;
		m_curFade += deltaTime;

		if (fadePercent > 1.f)
			fadePercent = 1.f;

		if (fadePercent <= 0.5f)
		{
			float fadeInPercent = fadePercent / 0.5f;
			currentText.setColor(sf::Color(255, 255, 255, fadeInPercent*255));
		}
		else
		{
			float fadeOutPercent = 1.f - ((fadePercent - 0.5f) / 0.5f);

			if (fadeOutPercent < 0)
				fadeOutPercent = 0;

			currentText.setColor(sf::Color(255, 255, 255, fadeOutPercent*255));

			if (fadeOutPercent <= 0)
			{
				++m_currentIndex;
				m_curFade = 0;

				if (m_currentIndex >= m_introTexts.size())
				{
					m_machine.popState();
					m_machine.pushState<Game>();
					return;
				}
			}
		}

	}
	


	if (!Notification::instance().isShown())
	{
		//m_machine.popState();
		//m_machine.pushState<Game>();
	}
}
void NewGameIntro::draw()
{
	for (std::size_t i = 0; i < m_introTexts.size(); i++)
		Renderer::instance().drawHUD(m_introTexts[i]);
	//Notification::instance().draw();
}

void NewGameIntro::onStop()
{

}
void NewGameIntro::onPlay()
{
	
}


void NewGameIntro::pushIntroText(const std::string &text)
{
	sf::Text introText;
	introText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	introText.setString(text);
	introText.setCharacterSize(150);
	introText.setColor(sf::Color(255, 255, 255, 0));
	introText.setPosition(
		m_window.getSize().x / 2.f - introText.getGlobalBounds().width / 2.f,
		m_window.getSize().y / 2.f - introText.getGlobalBounds().height / 2.f);

	m_introTexts.push_back(introText);
}