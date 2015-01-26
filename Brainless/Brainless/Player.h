#ifndef _PLAYER
#define _PLAYER

#include "Object.h"

// byt namn sen?
class Player : public Object
{
public:
	Player(float startX, float startY);
	void update();
protected:
	float m_speedX;
	float m_speedY;

private:
};



#endif
