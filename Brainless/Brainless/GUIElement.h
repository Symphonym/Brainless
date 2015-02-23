#ifndef INCLUDED_GUIELEMENT_H
#define INCLUDED_GUIELEMENT_H

#include <SFML\Graphics.hpp>
#include <string>

class GUIElement
{
public:

	virtual bool getReleased(const sf::RenderWindow &window) = 0;
	virtual std::string getType() = 0;

	virtual void draw() = 0;
};

#endif