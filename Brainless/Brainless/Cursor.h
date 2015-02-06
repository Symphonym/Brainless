#ifndef INCLUDED_CURSOR_H
#define INCLUDED_CURSOR_H

#include <SFML\Graphics.hpp>
#include <string>

class Game;
class Cursor
{
public:

	void changeCursor(const std::string &cursorTextureName);
	void update(Game &game);
	void draw();

	void resetToDefault();

	static Cursor& instance();

private:

	Cursor();

	sf::Sprite m_cursorSprite;
};

#endif