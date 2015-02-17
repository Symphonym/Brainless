#include "SpiritBar.h"
#include "Renderer.h"
#include "ResourceLoader.h"
#include "Utility.h"

SpiritBar::SpiritBar()
:
m_value(0),
m_maxValue(0)
{
	sf::Image barImg;
	barImg.create(1, 30, sf::Color::Cyan);

	m_barTexture.loadFromImage(barImg);

	m_barSprite.setTexture(m_barTexture);
	m_barBg.setTexture(m_barTexture);
	m_barBg.setColor(sf::Color::Color(255, 255, 255, 70));

	m_background.setTexture(ResourceLoader::instance().retrieveTexture("SpiritBarBG"));

	setPosition(sf::Vector2f(0, 0));
}

void SpiritBar::setPosition(const sf::Vector2f &position)
{
	m_background.setPosition(position);
	m_barSprite.setPosition(position.x + 35.f, position.y + 5.f);
	m_barBg.setPosition(position.x + 35.f, position.y + 5.f);
}
void SpiritBar::setValue(int value)
{
	m_value = value;

	m_value = Utility::clampValue<int>(m_value, 0, m_maxValue);
}
void SpiritBar::setMaxValue(int maxValue)
{
	m_maxValue = maxValue;
	m_value = maxValue;
	m_barBg.setScale(maxValue, 1);
	//m_background.scale(
	//	static_cast<float>(m_maxValue + 10.f) / m_background.getGlobalBounds().width,
	//	static_cast<float>(m_maxValue + 10.f) / m_background.getGlobalBounds().height);
}
void SpiritBar::addValue(int value)
{
	m_value += value;
	m_value = Utility::clampValue<int>(m_value, 0, m_maxValue);
}
void SpiritBar::removeValue(int value)
{
	m_value -= value;
	m_value = Utility::clampValue<int>(m_value, 0, m_maxValue);
}

int SpiritBar::getValue() const
{
	return m_value;
}
int SpiritBar::getMaxValue() const
{
	return m_maxValue;
}
bool SpiritBar::isFull() const
{
	return m_value >= m_maxValue;
}
bool SpiritBar::isEmpty() const
{
	return m_value == 0;
}

sf::Vector2i SpiritBar::getSize() const
{
	return sf::Vector2i(m_background.getGlobalBounds().width, m_background.getGlobalBounds().height);
}

void SpiritBar::draw()
{
	m_barSprite.setScale(m_value, 1);

	// m_background.x * A = B
	Renderer::instance().drawHUD(m_background);
	Renderer::instance().drawHUD(m_barBg);
	Renderer::instance().drawHUD(m_barSprite);
}