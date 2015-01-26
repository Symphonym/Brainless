#ifndef _PLAYER
#define _PLAYER

#include "Unit.h"

#include <SFML\Graphics.hpp>

class Player : Unit
{
public:
	Player(float startX, float startY);
	Player(float startX, float startY, int m_width, int m_height);

protected:

private:
};



#endif
