#include "Item.h"
#include "ResourceLoader.h"

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

sf::Sprite& Item::getSprite()
{
	return m_sprite;
}
int Item::getID() const
{
	return m_id;
}
int Item::getSyncID() const
{
	return m_syncID;
}