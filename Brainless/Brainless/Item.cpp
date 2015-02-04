#include "Item.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "Constants.h"

Item::Item(const std::string &textureName, int id, int syncID, CombineData combineData)
:
m_lootable(false),
m_usable(false),
m_collidable(false),
m_collisionOffset(0, 0),
m_collisionSize(0, 0),
m_useString(Constants::CantUseString),
m_pickupString(Constants::CantPickUpString),
m_examineString("A pretty normal object, nothing out of the ordinary"),
m_id(id),
m_syncID(syncID),
m_combineData(combineData)
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
	Renderer::instance().drawDepth(m_sprite);
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
const CombineData& Item::getCombineData() const
{
	return m_combineData;
}

bool Item::isLootable() const
{
	return m_lootable;
}
bool Item::isUsable() const
{
	return m_usable;
}
bool Item::isCollidable() const
{
	return m_collidable;
}
std::string Item::getUseString() const
{
	return m_useString;
}
std::string Item::getPickupString() const
{
	return m_pickupString;
}
std::string Item::getExamineString() const
{
	return m_examineString;
}
sf::FloatRect Item::getCollisionBounds() const
{
	return sf::FloatRect(getPosition().x + m_collisionOffset.x, getPosition().y + m_collisionOffset.y, m_collisionSize.x, m_collisionSize.y);
}

CombineData::CombineData(int targetIDIDParam, int productItemIDParam)
:
targetID(targetIDIDParam),
productItemID(productItemIDParam)
{

}




DefaultItem::DefaultItem(const std::string &textureName, int id, int syncID, CombineData combineData)
:
Item(textureName, id, syncID, combineData)
{
	m_lootable = true;
};

Item* DefaultItem::clone()
{
	return new DefaultItem(*this);
}







