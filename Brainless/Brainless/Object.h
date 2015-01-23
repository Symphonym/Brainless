#ifndef _OBJECT
#define _OBJECT

#include <SFML\Graphics.hpp>

class Object
{
public:
	Object(float startX, float startY);
	Object(float startX, float startY, int m_width, int m_height);
	virtual void update();
	sf::Sprite getSprite();
	void setTexture(sf::Texture texture);
protected:
	float m_positionX;
	float m_positionY;
	float m_speedX;
	float m_speedY;
	int m_width;
	int m_height;
	sf::Sprite m_sprite;
	// imagevector?
	// imagePosition
	// imageSize
private:
};



#endif
