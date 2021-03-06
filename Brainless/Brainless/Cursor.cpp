#include "Cursor.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "Game.h"

Cursor::Cursor()
{
	changeCursor("DefaultCursor");
}

void Cursor::changeCursor(const std::string &cursorTextureName)
{
	m_cursorSprite.setTexture(ResourceLoader::instance().retrieveTexture(cursorTextureName));
	m_cursorSprite.setOrigin(0, 0);
}
void Cursor::update(const sf::RenderWindow &window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	m_cursorSprite.setPosition(mousePos.x, mousePos.y);
}
void Cursor::draw()
{
	Renderer::instance().drawHUD(m_cursorSprite);
}

void Cursor::resetToDefault()
{
	changeCursor("DefaultCursor");
}

Cursor& Cursor::instance()
{
	static Cursor cursor;
	return cursor;
}


	sf::Sprite m_cursorSprite;