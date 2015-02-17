#ifndef INCLUDED_SPIRIT_BAR_H
#define INCLUDED_SPIRIT_BAR_H

#include <SFML\Graphics.hpp>

class SpiritBar
{
public:

	SpiritBar();

	void setPosition(const sf::Vector2f &position);

	void setValue(int value);
	void setMaxValue(int maxValue);

	void addValue(int value);
	void removeValue(int value);
	
	bool isFull() const;
	bool isEmpty() const;

	int getValue() const;
	int getMaxValue() const;

	sf::Vector2i getSize() const;

	void draw();

private:

	sf::Sprite m_background;
	sf::Sprite m_barSprite;
	sf::Sprite m_barBg;

	sf::Texture m_barTexture;
	
	int m_value, m_maxValue;
};

#endif