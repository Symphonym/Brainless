#include "BookShelfItem.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "ItemDatabase.h"
#include "Unit.h"
#include "Game.h"
#include <iostream>

BookShelfItem::BookShelfItem(int id)
:
Item("Bookshelf", "bookShelf", "bookShelf", id)
{
	m_usable = true;
	m_collidable = true;

	m_collisionBounds = sf::FloatRect(61, 12, 136, 229);
	m_interactBounds = m_collisionBounds;
}

void BookShelfItem::onExamine()
{
	m_examineString = "It looks like a shelf, it appears to be filled with books. \nBooks are collections of paper which contain information in the form of text.";
}

void BookShelfItem::draw()
{
	Renderer::instance().drawDepth(getSprite());
}

Item* BookShelfItem::clone()
{
	return new BookShelfItem(*this);
}