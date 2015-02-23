#include "GhostItem.h"
#include "ConversationBox.h"
#include "Renderer.h"
#include <math.h>
#include "Game.h"
#include "Notification.h"

GhostItem::GhostItem(const std::string &dialogFile, int id)
:
Item("Ghost","GhostItem", "GhostItem", id)
{
	m_dialog.loadDialogFile(dialogFile);

	m_lootable = false;
	m_usable = true;
	m_pickupString = "It would be unwise to try to pick up a ghost";
	m_examineString = "It's a ghost alright, maybe I should talk to it";
	getSprite().setTextureRect(sf::IntRect(0,0,256,256));
}

void GhostItem::update(float deltaTime, Game &game)
{
	m_currentFrame += deltaTime*2;
	int frame_xpos = ((((int)floor(m_currentFrame))%14)%8);
	int frame_ypos = floor(((int)floor(m_currentFrame) % 14) / 8);
	getSprite().setTextureRect(sf::IntRect(frame_xpos*256,frame_ypos*256,256,256));
}
void GhostItem::draw()
{
	// TODO ANIMATIONS?
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
