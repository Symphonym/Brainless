#include "Notification.h"
#include "ResourceLoader.h"

Notification::Notification()
:
m_curDelay(0),
m_maxDelay(1),
m_textBox(sf::Vector2f(0, 0), sf::Vector2f(300, 100), ResourceLoader::instance().retrieveFont("DefaultFont"))
{

}

// Text to write
void Notification::write(const std::string &text)
{
	m_textBox.Type(text, 5000.f, sf::Color::White, 20);
	m_curDelay = 0;
}

// Time the text will remain after being written
void Notification::setFinishDelay(float delay)
{
	m_maxDelay = delay;
	m_curDelay = 0;
}

void Notification::setPosition(const sf::Vector2f &position)
{
	m_textBox.setPosition(position);
}

void Notification::update(float deltaTime)
{
	m_textBox.Update(deltaTime);

	if (m_textBox.isFinished())
	{
		m_curDelay += deltaTime;

		if (m_curDelay >= m_maxDelay)
		{
			m_curDelay = m_curDelay - m_maxDelay;
			m_textBox.Type("");
		}
	}
	else
		m_curDelay = 0;
}
void Notification::draw()
{
	m_textBox.Draw(false);
}

Notification& Notification::instance()
{
	static Notification not;
	return not;
}
