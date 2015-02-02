#include "Item.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "Constants.h"

Item::Item(const std::string &textureName, int id, int syncID, CombineData combineData)
:
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

std::string Item::onExamine() const
{
	return "Nothing interesting.";
}

std::string Item::onUse() const
{
	return Constants::CantUseString;
}

std::string Item::onPickUp() const
{
	return Constants::CantPickUpString;
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

};

Item* DefaultItem::clone()
{
	return new DefaultItem(*this);
}







