#ifndef _PLAYER
#define _PLAYER

#include "Unit.h"

#include <SFML\Graphics.hpp>

class Player : public Unit
{
public:
	Player(float startX, float startY, float maxSpeedX, float maxSpeedY);
	Player(float startX, float startY, int width, int height, float maxSpeedX, float maxSpeedY);
	/* kollar efter keyboardinput, och eventuel slide*/
	void checkPlayerInput();
protected:

private:
};



#endif
