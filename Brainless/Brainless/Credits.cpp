#include "Credits.h"
#include "Renderer.h"
#include "ResourceLoader.h"

Credits::Credits(StateMachine &machine)
:
State(machine)
{
	ResourceLoader::instance().loadResourceFile("loadfiles/ResourceLoad_MainMenu.txt");

	m_background.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background"));
	m_backgroundElement_hair.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background_Elements"));
	m_backgroundElement_grass1.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background_Elements"));
	m_backgroundElement_grass2.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background_Elements"));
	m_backgroundElement_grass3.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background_Elements"));
	m_backgroundElement_grass4.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background_Elements"));
	m_backgroundElement_grass5.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background_Elements"));
	m_backgroundElement_scarf.setTexture(ResourceLoader::instance().retrieveTexture("Menu_Background_Elements"));

	m_backgroundElement_hair.setPosition(8, 94);
	m_backgroundElement_grass1.setPosition(1075, 569);
	m_backgroundElement_grass2.setPosition(710, 569);
	m_backgroundElement_grass3.setPosition(400, 569);
	m_backgroundElement_grass4.setPosition(140, 569);
	m_backgroundElement_grass5.setPosition(-100, 565);
	m_backgroundElement_scarf.setPosition(-15, 166);

	m_Animation_hair.loop(0, 5, 0, 4.5f);
	m_Animation_grass1.loop(0, 5, 1, 4.5f);
	m_Animation_grass2.loop(0, 5, 2, 4.5f);
	m_Animation_grass3.loop(0, 5, 3, 4.5f);
	m_Animation_grass4.loop(0, 5, 4, 4.5f);
	m_Animation_grass5.loop(0, 5, 5, 4.5f);
	m_Animation_scarf.loop(0, 5, 6, 4.5f);

	pushCreditsParagraph("Programming", 100);
	pushCreditsParagraph("Jakob Larsson");
	pushCreditsParagraph("Kevin Lövgren", 15);
	pushCreditsParagraph("Tobias Melker");
	pushCreditsParagraph("Svante Almbring");


	pushCreditsParagraph("Graphics", 100);
	pushCreditsParagraph("Johan Wibrink");
	pushCreditsParagraph("Olof Andersson");
	pushCreditsParagraph("Linda Inghammar");
	pushCreditsParagraph("Fredrik Olsson");
	pushCreditsParagraph("Emelie Tordelöv");
	pushCreditsParagraph("Pernilla Larsson");

	pushCreditsParagraph("Game writing", 100);
	pushCreditsParagraph("Linnea Thimrén");
	pushCreditsParagraph("Robin Norén");

	pushCreditsParagraph("Design", 100);
	pushCreditsParagraph("Fredrik Söderberg");

	pushCreditsParagraph("Ljud", 100);
	pushCreditsParagraph("Simon Enström");
	pushCreditsParagraph("Fredrik Larsson");

	pushCreditsParagraph("Musik", 100);
	pushCreditsParagraph("Rasmus Andersparr");
	pushCreditsParagraph("Gustaf Yngström");

	resetCreditsPosition();

	m_creditsBG.setTexture(ResourceLoader::instance().retrieveTexture("CreditsBG"));
}

void Credits::events(const sf::Event &event)
{

}
void Credits::update(float deltaTime)
{
	m_backgroundElement_hair.setTextureRect(m_Animation_hair.getRectangle(deltaTime));
	m_backgroundElement_grass1.setTextureRect(m_Animation_grass1.getRectangle(deltaTime));
	m_backgroundElement_grass2.setTextureRect(m_Animation_grass2.getRectangle(deltaTime));
	m_backgroundElement_grass3.setTextureRect(m_Animation_grass3.getRectangle(deltaTime));
	m_backgroundElement_grass4.setTextureRect(m_Animation_grass4.getRectangle(deltaTime));
	m_backgroundElement_grass5.setTextureRect(m_Animation_grass5.getRectangle(deltaTime));
	m_backgroundElement_scarf.setTextureRect(m_Animation_scarf.getRectangle(deltaTime));

	for (std::size_t i = 0; i < m_creditText.size(); i++)
	{
		sf::Text &text = m_creditText[i];

		text.setPosition(
			m_window.getSize().x / 2.f - text.getGlobalBounds().width / 2.f,
			text.getPosition().y - 200.f * deltaTime);

		if (i == m_creditText.size() - 1 && text.getPosition().y < 0)
			resetCreditsPosition();
	}


}
void Credits::draw()
{
	Renderer::instance().drawHUD(m_background);
	Renderer::instance().drawHUD(m_backgroundElement_hair);
	Renderer::instance().drawHUD(m_backgroundElement_grass1);
	Renderer::instance().drawHUD(m_backgroundElement_grass2);
	Renderer::instance().drawHUD(m_backgroundElement_grass3);
	Renderer::instance().drawHUD(m_backgroundElement_grass4);
	Renderer::instance().drawHUD(m_backgroundElement_grass5);
	Renderer::instance().drawHUD(m_backgroundElement_scarf);

	Renderer::instance().drawHUD(m_creditsBG);

	for (std::size_t i = 0; i < m_creditText.size(); i++)
		Renderer::instance().drawHUD(m_creditText[i]);
}

void Credits::onStop()
{

}
void Credits::onPlay()
{

}

void Credits::resetCreditsPosition()
{
	
	for (std::size_t i = 0; i < m_creditText.size(); i++)
	{
		m_creditText[i].setPosition(
			m_window.getSize().x / 2.f - m_creditText[i].getGlobalBounds().width / 2.f,
			m_window.getSize().y + 50.f + i * 150.f);
	}
}
void Credits::pushCreditsParagraph(const std::string &text, unsigned int charSize)
{
	sf::Text creditText;
	creditText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	creditText.setString(text);
	creditText.setCharacterSize(charSize);
	m_creditText.push_back(creditText);
}