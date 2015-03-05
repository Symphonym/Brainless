#include "CrateItem.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "ItemDatabase.h"
#include "Unit.h"
#include "Game.h"
#include <iostream>

CrateItem::CrateItem(int id)
:
Item("Crate", "Crate", "Crate", id)
{
	m_usable = true;
	m_collidable = true;

	m_collisionBounds = sf::FloatRect(24, 24, 82, 88);
	m_interactBounds = m_collisionBounds;
}

void CrateItem::onExamine()
{
	m_examineString = "I should jump on this..";
}

void CrateItem::draw()
{
	Renderer::instance().drawDepth(getSprite());
}

Item* CrateItem::clone()
{
	return new CrateItem(*this);
}