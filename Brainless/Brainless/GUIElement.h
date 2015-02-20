#ifndef INCLUDED_GUIELEMENT_H
#define INCLUDED_GUIELEMENT_H

#include <string>

class GUIElement
{
public:

	virtual bool getReleased(const sf::Vector2i &mousePos) = 0;
	virtual std::string getType() = 0;

	virtual void draw() = 0;
};

#endif