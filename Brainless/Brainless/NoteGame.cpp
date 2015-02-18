#include "NoteGame.h"
#include "ResourceLoader.h"
#include "ArcadeMachine.h"
#include "Renderer.h"

NoteGame::NoteGame(ArcadeMachine &machine)
:
ArcadeGame(machine, "Åkes Melodisak"),
m_score(0)
{
	m_scoreText.setFont(ResourceLoader::instance().retrieveFont("DefaultFont"));
	m_scoreText.setPosition(m_machine.getScreenPos().x + 100.f, m_machine.getScreenPos().y + m_machine.getScreenSize().y - 100.f);
	m_scoreText.setString("Score: 0");
}

void NoteGame::events(const sf::Event &event)
{

}
void NoteGame::update(float deltaTime)
{
	m_scoreText.setString("Score: " + std::to_string(m_score));

	// Spawn new notes
	if (m_aCurDelay >= m_aMaxDelay)
	{
		m_aCurDelay = m_aCurDelay - m_aMaxDelay;
		
		sf::Sprite aNote;
	}
	if (m_wCurDelay >= m_wMaxDelay)
	{
		m_wCurDelay = m_wCurDelay - m_wMaxDelay;
	}
	if (m_dCurDelay >= m_dMaxDelay)
	{
		m_dCurDelay = m_dCurDelay - m_dMaxDelay;
	}
}
void NoteGame::draw()
{
	Renderer::instance().drawHUD(m_scoreText);
}