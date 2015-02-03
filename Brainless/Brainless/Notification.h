#ifndef INCLUDED_NOTIFICATION_H
#define INCLUDED_NOTIFICATION_H

#include "WrappedText.h"

class Notification
{
public:

	// Text to write
	void write(const std::string &text);

	// Time the text will remain after being written
	void setFinishDelay(float delay);

	void setPosition(const sf::Vector2f &position);

	void update(float deltaTime);
	void draw();

	static Notification& instance();

private:

	Notification();

	float m_curDelay;
	float m_maxDelay;
	WrappedText m_textBox;
};

#endif