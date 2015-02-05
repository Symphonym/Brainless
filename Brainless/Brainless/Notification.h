#ifndef INCLUDED_NOTIFICATION_H
#define INCLUDED_NOTIFICATION_H

#include <SFML\Graphics.hpp>
#include "WrappedText.h"

class Notification
{
public:

	// Text to write
	void write(const std::string &text);

	// Time the text will remain after being written
	void setFinishDelay(float delay);

	void setPosition(const sf::Vector2f &position);

	void update(float deltaTime, const sf::RenderWindow &gameWindow);
	void draw();

	bool isShown() const;

	static Notification& instance();

private:

	Notification();

	bool m_isShown;
	sf::Sprite m_background;

	float m_curDelay;
	float m_maxDelay;
	WrappedText m_textBox;
};

#endif