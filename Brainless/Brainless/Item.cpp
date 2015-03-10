#include "Item.h"
#include "ResourceLoader.h"
#include "Renderer.h"
#include "Constants.h"

Item::Item(const std::string &itemName, const std::string &inventoryTextureName, const std::string &textureName, int id)
:
m_lootable(false),
m_usable(false),
m_collidable(false),
m_solid(true),
m_renderingMode(RenderingModes::Depth),
m_interactDistance(Constants::InteractDistance, Constants::InteractDistance),
m_interactBounds(0, 0, 0, 0),
m_collisionBounds(0, 0, 0, 0),
m_useString(Constants::CantUseString),
m_pickupString(Constants::CantPickUpString),
m_examineString("A pretty normal object, nothing out of the ordinary"),
m_flyingOff(false),
m_markedForDestruction(false),
m_itemName(itemName),
m_id(id),
m_syncID(-1),
m_speed(sf::Vector2f(0.f,0.f))
{
	m_sprite.setTexture(ResourceLoader::instance().retrieveTexture(textureName));
	m_inventorySprite.setTexture(ResourceLoader::instance().retrieveTexture(inventoryTextureName));

	// Default collision box is size of sprite
	m_interactBounds = sf::FloatRect(0, 0, m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height);
}

void Item::flyOff()
{
	m_collidable = false;
	m_flyingOff = true;
	m_flyingOffLifeTime = 1.f;
}
void Item::updateFlyoff(float deltaTime)
{
	if (!m_flyingOff)
		return;

	m_renderingMode = RenderingModes::Above;

	m_flyingOffLifeTime -= deltaTime;
	sf::Vector2f velocity = sf::Vector2f(500, -500);
	setPosition(getPosition() + (velocity*deltaTime));

	float scale = getSprite().getScale().x;
	scale -= deltaTime;

	if (scale <= 0.1f)
		scale = 0.1f;

	getSprite().setScale(scale, scale);
	getSprite().rotate(deltaTime*300.f);

	if (m_flyingOffLifeTime <= 0)
		markForDestruction();
}

void Item::serialize(std::ofstream &writer) const
{
	// This is so the reader can quickly grab the item from the database
	writer << m_id << std::endl;

	writer << m_syncID << std::endl;
	writer << getPosition().x << std::endl;
	writer << getPosition().y << std::endl;
	writer << m_interactBounds.left << std::endl;
	writer << m_interactBounds.top << std::endl;
	writer << m_interactBounds.width << std::endl;
	writer << m_interactBounds.height << std::endl;
	writer << m_collisionBounds.left << std::endl;
	writer << m_collisionBounds.top << std::endl;
	writer << m_collisionBounds.width << std::endl;
	writer << m_collisionBounds.height << std::endl;
	writer << m_lootable << std::endl;
	writer << m_usable << std::endl;
	writer << m_collidable << std::endl;
	writer << m_solid << std::endl;
	writer << m_flyingOff << std::endl;
	writer << m_markedForDestruction << std::endl;
	writer << static_cast<int>(m_renderingMode) << std::endl;
}
void Item::deserialize(std::ifstream &reader)
{
	// ID is read initially outside this function to grab the item from the database
	float posX = 0, posY = 0;
	reader >> m_syncID >> posX >> posY;

	setPosition(sf::Vector2f(posX, posY));

	reader >> m_interactBounds.left >> m_interactBounds.top >> m_interactBounds.width >> m_interactBounds.height;
	reader >> m_collisionBounds.left >> m_collisionBounds.top >> m_collisionBounds.width >> m_collisionBounds.height;
	reader >> m_lootable >> m_usable >> m_collidable >> m_solid;
	reader >> m_flyingOff >> m_markedForDestruction;

	if (m_flyingOff)
		markForDestruction();

	int renderMode = 0;
	reader >> renderMode;
	m_renderingMode = static_cast<RenderingModes>(renderMode);
}

void Item::markForDestruction()
{
	m_markedForDestruction = true;
}


void Item::setPosition(const sf::Vector2f &pos)
{
	m_sprite.setPosition(pos);
	m_inventorySprite.setPosition(pos);
	onPositionChanged();
}
sf::Vector2f Item::getPosition() const
{
	return m_sprite.getPosition();
}

void Item::inventoryDraw(bool drawInWorldSpace)
{
	if (drawInWorldSpace)
		Renderer::instance().drawAbove(m_inventorySprite);
	else
		Renderer::instance().drawHUD(m_inventorySprite);
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
sf::Sprite& Item::getInventorySprite()
{
	return m_inventorySprite;
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

bool Item::isMarkedForDestruction() const
{
	return m_markedForDestruction;
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
std::string Item::getToggleString() const
{
	return "";
}
sf::FloatRect Item::getCollisionBounds() const
{
	return sf::FloatRect(getPosition().x + m_collisionBounds.left, getPosition().y + m_collisionBounds.top, m_collisionBounds.width, m_collisionBounds.height);
}
sf::FloatRect Item::getInteractBounds() const
{
	return sf::FloatRect(getPosition().x + m_interactBounds.left, getPosition().y + m_interactBounds.top, m_interactBounds.width, m_interactBounds.height);
}
sf::Vector2f Item::getInteractDistance() const
{
	return m_interactDistance;
}
sf::Vector2f Item::getSpeed() const
{
	return m_speed;
}




DefaultItem::DefaultItem(const std::string &textureName, int id)
:
Item("DefaultItem", textureName, "DEFAULT_INVENTORY_NAME", id)
{
	m_lootable = true;
};

Item* DefaultItem::clone()
{
	return new DefaultItem(*this);
}







