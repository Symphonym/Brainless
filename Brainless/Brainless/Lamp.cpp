#include "Lamp.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "ItemDatabase.h"
#include "Unit.h"
#include "Game.h"
#include <iostream>

Lamp::Lamp(int id)
:
Item("Lamp", "Lamp", "Lamp", id)
{
	m_usable = false;
	m_collidable = true;

	m_collisionBounds = sf::FloatRect(20,20,226, 33);
	m_interactBounds = m_collisionBounds;
}

void Lamp::onExamine()
{
	m_examineString = "TEMP";
}

void Lamp::draw()
{
	Renderer::instance().drawDepth(getSprite());
}

Item* Lamp::clone()
{
	return new Lamp(*this);
}