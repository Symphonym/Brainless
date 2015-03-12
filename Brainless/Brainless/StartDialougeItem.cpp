#include "StartDialougeItem.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "ItemDatabase.h"
#include "Unit.h"
#include "Game.h"
#include "DialogTree.h"
#include "ConversationBox.h"
#include <iostream>

StartDialougeItem::StartDialougeItem(int id)
:
Item("StartDialougeItem", "DialougeItem", "DialougeItem", id)
{
	m_usable = false;
	m_collidable = false;

	m_collisionBounds = sf::FloatRect(0, 0, 200, 200);
	m_interactBounds = m_collisionBounds;
}

bool StartDialougeItem::onCollisionWithUnit(Unit &unit, Game &game)
{
	// TODO FIX THIS, IT DOES NOT WANT TO FIND/OPEN THE FILE, DA FUQ, FIX PLZ
	DialogTree tree;
	tree.loadDialogFile("dialogues/intro.txt");
	tree.resetDialog();

	sf::Vector2i onScreenPos = game.getWindow().mapCoordsToPixel(getPosition());

	ConversationBox::instance().setPosition(sf::Vector2f(onScreenPos.x, onScreenPos.y));
	ConversationBox::instance().setDialog(tree);
	ConversationBox::instance().setShown(true);

	return true;
}

Item* StartDialougeItem::clone()
{
	return new StartDialougeItem(*this);
}
