#ifndef INCLUDED_GUIELEMENT_H
#define INCLUDED_GUIELEMENT_H

#include <string>

class GUIElement
{
	~GUIElement();

	virtual bool getReleased() = 0;
	virtual std::string getType() = 0;
};

#endif