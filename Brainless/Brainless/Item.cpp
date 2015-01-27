#include "Item.h"
#include "ResourceLoader.h"
#include "Renderer.h"

Item::Item(const std::string &textureName, int id, int syncID)
:
m_id(id),
m_syncID(syncID)
{
	m_sprite.setTexture(ResourceLoader::instance().retrieveTexture(textureName));
}

void Item::setPosition(const sf::Vector2f &pos)
{
	m_sprite.setPosition(pos);
}
sf::Vector2f Item::getPosition() const
{
	return m_sprite.getPosition();
}

void Item::draw()
{
	//Renderer::instance().draw(m_sprite, m_sprite); TODO UNCOMMENT THIS AND REMOVE DRAWTMSPECIAL
	//Renderer::instance().drawTMSpecial(m_sprite, m_sprite.getGlobalBounds());
	Renderer::instance().drawByBounds(m_sprite, m_sprite.getGlobalBounds());
}

sf::Sprite& Item::getSprite()
{
	return m_sprite;
}
int Item::getID() const
{
	return m_id;
}

void Item::setSyncID(int id)
{
	m_syncID = id;
}

int Item::getSyncID() const
{
	return m_syncID;
}





DefaultItem::DefaultItem(const std::string &textureName, int id, int syncID)
:
Item(textureName, id, syncID)
{

};

Item* DefaultItem::clone()
{
	return new DefaultItem(*this);
}