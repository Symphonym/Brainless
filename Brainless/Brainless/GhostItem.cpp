#include "GhostItem.h"
#include "ConversationBox.h"
#include "Renderer.h"

GhostItem::GhostItem(const std::string &dialogFile, int id)
:
Item("GhostTest", id, -1)
{
	m_dialog.loadDialogFile(dialogFile);

	m_lootable = false;
	m_usable = true;
	m_pickupString = "It would be unwise to try to pick up a ghost";
	m_examineString = "It's a ghost alright, maybe I should talk to it";
}

void GhostItem::update(float deltaTime)
{
}
void GhostItem::draw()
{
	// TODO ANIMATIONS?
	Renderer::instance().drawDepth(getSprite());
}

void GhostItem::onUse(const sf::RenderWindow &window)
{
	sf::Vector2i onScreenPos = window.mapCoordsToPixel(getPosition());

	m_dialog.resetDialog();
	ConversationBox::instance().setPosition(sf::Vector2f(onScreenPos.x, onScreenPos.y));
	ConversationBox::instance().setDialog(m_dialog);
	ConversationBox::instance().setShown(true);
}

Item* GhostItem::clone()
{
	return new GhostItem(*this);
}
