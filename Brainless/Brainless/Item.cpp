#include "Item.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "Constants.h"

Item::Item(const std::string &itemName, const std::string &textureName, int id, CombineData combineData)
:
m_lootable(false),
m_usable(false),
m_collidable(false),
m_solid(true),
m_renderingMode(RenderingModes::Depth),
m_interactDistance(Constants::InteractDistance, Constants::InteractDistance),
m_collisionOffset(0, 0),
m_collisionSize(0, 0),
m_useString(Constants::CantUseString),
m_pickupString(Constants::CantPickUpString),
m_examineString("A pretty normal object, nothing out of the ordinary"),
m_itemName(itemName),
m_id(id),
m_syncID(-1),
m_combineData(combineData),
m_speed(sf::Vector2f(0,0))
{
	m_sprite.setTexture(ResourceLoader::instance().retrieveTexture(textureName));
}

void Item::serialize(std::ofstream &writer) const
{
	// This is so the reader can quickly grab the item from the database
	writer << m_id << std::endl;

	writer << m_syncID << std::endl;
	writer << getPosition().x << std::endl;
	writer << getPosition().y << std::endl;
	writer << m_lootable << std::endl;
	writer << m_usable << std::endl;
	writer << m_collidable << std::endl;
	writer << static_cast<int>(m_renderingMode) << std::endl;
}
void Item::deserialize(std::ifstream &reader)
{
	// ID is read initially outside this function to grab the item from the database
	float posX = 0, posY = 0;
	reader >> m_syncID >> posX >> posY;

	setPosition(sf::Vector2f(posX, posY));

	reader >> m_lootable >> m_usable >> m_collidable;

	int renderMode = 0;
	reader >> renderMode;
	m_renderingMode = static_cast<RenderingModes>(renderMode);
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
	if (m_renderingMode == RenderingModes::Above)
		Renderer::instance().drawAbove(m_sprite);
	else if (m_renderingMode == RenderingModes::Depth)
		Renderer::instance().drawDepth(m_sprite);
	else if (m_renderingMode == RenderingModes::Behind)
		Renderer::instance().drawBehind(m_sprite);
}

sf::Sprite& Item::getSprite()
{
	return m_sprite;
}
std::string Item::getName() const
{
	return m_itemName;
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
bool Item::isSolid() const
{
	return m_solid;
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
sf::Vector2f Item::getInteractDistance() const
{
	return m_interactDistance;
}
sf::Vector2f Item::getSpeed() const
{
	return m_speed;
}

CombineData::CombineData(int targetIDIDParam, int productItemIDParam)
:
targetID(targetIDIDParam),
productItemID(productItemIDParam)
{

}




DefaultItem::DefaultItem(const std::string &textureName, int id, CombineData combineData)
:
Item("DefaultItem", textureName, id, combineData)
{
	m_lootable = true;
};

Item* DefaultItem::clone()
{
	return new DefaultItem(*this);
}







