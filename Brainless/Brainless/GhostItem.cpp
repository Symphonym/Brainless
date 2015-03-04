#include "GhostItem.h"
#include "ConversationBox.h"
#include "Renderer.h"
#include <math.h>
#include "Game.h"
#include "Notification.h"

#define SPRITESIZE 256

GhostItem::GhostItem(const std::string &dialogFile, int id)
:
m_animation(SPRITESIZE, SPRITESIZE),
Item("Ghost","GhostItem", "GhostItem", id)
{
	m_dialog.loadDialogFile(dialogFile);
	m_interactBounds = sf::FloatRect(65, 75, 150, 150);
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

Item* GhostItem::clone()
{
	return new GhostItem(*this);
}
