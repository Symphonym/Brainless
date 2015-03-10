#include "GhostItem.h"
#include "ConversationBox.h"
#include "Renderer.h"
#include <math.h>
#include <string>
#include "Game.h"
#include "Notification.h"

#define SPRITESIZE 256

GhostItem::GhostItem(int id, int ghostID)
:
m_ghostID(ghostID),
m_animation(SPRITESIZE, SPRITESIZE),
Item("Ghost","GhostItem", "GhostItem", id)
{
	m_interactBounds = sf::FloatRect(65, 75, 150, 150);
	m_collisionBounds = sf::FloatRect(65, 75, 150, 150);
	m_lootable = false;
	m_usable = true;
	m_pickupString = "It would be unwise to try to pick up a ghost";
	m_examineString = "It's a spirit! Maybe I should try to talk to it?";
	m_interactDistance = sf::Vector2f(500, 500);
	getSprite().setTextureRect(sf::IntRect(0,0,256,256));

	m_animation.loop(0, 5, 0, 1, 5);
	getSprite().setTextureRect(m_animation.getRectangle(0));
}

void GhostItem::update(float deltaTime, Game &game)
{

	getSprite().setTextureRect(m_animation.getRectangle(deltaTime));
}
void GhostItem::draw()
{
	Renderer::instance().drawDepth(getSprite());
}

void GhostItem::onUse(Game &game)
{
	m_dialog.loadDialogFile("dialogues/Ghost" + std::to_string(m_ghostID) + ".txt");
	// You can't talk to the ghost if you don't have spirit power
	if (game.getSpiritBar().getValue() < Constants::SpiritGhostCost)
	{
		Notification::instance().write("I don't have enough spirit within me to communicate with the ghost");
		return;
	}
	else
		game.getSpiritBar().removeValue(Constants::SpiritGhostCost);

	sf::Vector2i onScreenPos = game.getWindow().mapCoordsToPixel(getPosition());

	m_dialog.resetDialog();
	ConversationBox::instance().setPosition(sf::Vector2f(onScreenPos.x, onScreenPos.y));
	ConversationBox::instance().setDialog(m_dialog);
	ConversationBox::instance().setShown(true);
}

bool GhostItem::onCollisionWithUnit(Unit &unit, Game &game)
{
	if (unit.getUnitType() == Unit::ID_Player)
	{
		if (m_ghostID == 0)
		{
			m_ghostID = -1;
		}
	}
	return false;
}

void GhostItem::serialize(std::ofstream &writer) const
{
	Item::serialize(writer);
	writer << m_ghostID << std::endl;
}

void GhostItem::deserialize(std::ifstream &reader)
{
	Item::deserialize(reader);
	reader >> m_ghostID;
}

bool GhostItem::onToggle(int direction)
{
	m_ghostID+=direction;
	return true;
}


std::string GhostItem::getToggleString() const
{
	return "Dialogue id: " + std::to_string(m_ghostID);
}

Item* GhostItem::clone()
{
	return new GhostItem(*this);
}

int GhostItem::getGhostID() const
{
	return m_ghostID;
}

void GhostItem::setGhostID(int ghostID)
{
	m_ghostID = ghostID;
}