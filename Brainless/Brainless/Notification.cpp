#include "Notification.h"
#include "ResourceLoader.h"
#include "Renderer.h"

Notification::Notification()
:
m_isShown(false),
m_curDelay(0),
m_maxDelay(1.f),
m_textBox(sf::Vector2f(0, 0), sf::Vector2f(300, 100), ResourceLoader::instance().retrieveFont("DefaultFont"))
{
	m_background.setTexture(ResourceLoader::instance().retrieveTexture("DialogBoxBackground"));
	m_textBox.setSize(sf::Vector2f(m_background.getGlobalBounds().width - 30.f, m_background.getGlobalBounds().height - 20.f));
	setPosition(sf::Vector2f(0, 0));
}

void Notification::write(const std::string &text)
{
	m_textBox.Type(text, 5000.f, sf::Color::White, 20);
	m_curDelay = 0;
	m_isShown = true;
	m_background.setColor(sf::Color::White);
}

void Notification::setFinishDelay(float delay)
{
	m_maxDelay = delay;
	m_curDelay = 0;
}

void Notification::setPosition(const sf::Vector2f &position)
{
	m_background.setPosition(position);
	m_textBox.setPosition(sf::Vector2f(m_background.getPosition().x + 20.f, m_background.getPosition().y + 10.f));
}

void Notification::update(float deltaTime, const sf::RenderWindow &gameWindow)
{
	if (m_isShown)
	{
		m_textBox.Update(deltaTime);
		sf::Vector2f pos(
			gameWindow.getSize().x / 2.f - m_background.getGlobalBounds().width / 2.f,
			gameWindow.getSize().y - m_background.getGlobalBounds().height);
		setPosition(pos);

		if (m_textBox.isFinished())
		{
			m_curDelay += deltaTime;
			float diff = m_curDelay / m_maxDelay;

			sf::Color fadedColor(255, 255, 255, 255 - 255 * diff);
			m_background.setColor(fadedColor);
			m_textBox.setTextColor(fadedColor);

			if (m_curDelay >= m_maxDelay)
			{
				m_curDelay = m_curDelay - m_maxDelay;
				//m_textBox.Type("");
				m_isShown = false;
			}
		}
		else
			m_curDelay = 0;
	}

}
void Notification::draw()
{
	if (m_isShown)
	{
		Renderer::instance().drawHUD(m_background);
		m_textBox.Draw(true);
	}
}

bool Notification::isShown() const
{
	return m_isShown;
}

Notification& Notification::instance()
{
	static Notification not;
	return not;
}
