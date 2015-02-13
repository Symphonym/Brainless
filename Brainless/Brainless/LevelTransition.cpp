#include "LevelTransition.h"
#include "Renderer.h"
#include "Utility.h"
#include "Game.h"

LevelTransition::LevelTransition(Game &game)
:
m_game(game),
m_isActive(false),
m_fadingOut(true),
m_curDelay(0),
m_maxDelay(1.f),
m_targetLevel(0)
{
	refreshTexture();
}

void LevelTransition::startTransition(int targetLevel,bool swapPosition, float fadeOutTime)
{
	m_isActive = true;
	m_fadingOut = true;
	m_curDelay = 0;
	m_maxDelay = fadeOutTime;
	m_targetLevel = targetLevel;
	m_swapPosition = swapPosition;
}

void LevelTransition::update(float deltaTime)
{
	if (m_isActive)
	{
		m_curDelay += deltaTime;

		float relativeAlpha = m_curDelay / m_maxDelay;
		relativeAlpha = Utility::clampValue<float>(relativeAlpha, 0, 1.f);

		// Reached end of delay
		if (m_curDelay >= m_maxDelay)
		{
			m_curDelay = m_curDelay - m_maxDelay;

			// If we're done fading in, the transition is finished
			if (!m_fadingOut)
			{
				m_isActive = false;
				return;
			}

			// Change level and start fading in after we've faded out
			else
			{
				m_game.changeLevel(m_targetLevel,m_swapPosition);
				m_fadingOut = false;
			}
		}

		if (m_fadingOut)
			m_coveringSprite.setColor(sf::Color::Color(0, 0, 0, relativeAlpha * 255));
		else
			m_coveringSprite.setColor(sf::Color::Color(0, 0, 0, 255 - relativeAlpha * 255));
	}
}
void LevelTransition::draw()
{
	if (m_isActive)
	{
		Renderer::instance().drawHUD(m_coveringSprite);
	}
}

// Refresh screen covering textures, this only needs to be run
// at startup or when the screen size changes
void LevelTransition::refreshTexture()
{
	sf::Image img;
	img.create(m_game.getWindow().getSize().x, m_game.getWindow().getSize().y, sf::Color::Black);
	m_coveringTexture.loadFromImage(img);
	m_coveringSprite.setTexture(m_coveringTexture);
}

bool LevelTransition::getActive() const
{
	return m_isActive;
};