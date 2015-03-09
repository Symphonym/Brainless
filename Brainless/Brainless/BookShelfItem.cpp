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
	m_examineString = "A chance to stock up on books! ...Oh, oh god, it's that one about sparkly vampires. Run, for gods sake, run!";
}

void BookShelfItem::draw()
{
	Renderer::instance().drawDepth(getSprite());
}

Item* BookShelfItem::clone()
{
	return new BookShelfItem(*this);
}