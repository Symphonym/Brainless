#ifndef _OBJECT
#define _OBJECT

#include <SFML\Graphics.hpp>

// byt namn sen?
class Object
{
public:
	Object(float startX, float startY);
	Object(float startX, float startY, int m_width, int m_height);
	virtual void update();
	sf::Sprite getSprite();
	void setTexture(sf::Texture& texture);
	void draw();
protected:
	float m_positionX;
	float m_positionY;
	int m_width;
	int m_height;
	sf::Sprite m_sprite;
private:
};



#endif
