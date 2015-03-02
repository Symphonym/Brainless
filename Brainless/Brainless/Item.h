#ifndef INCLUDED_ITEM_H
#define INCLUDED_ITEM_H

#include <string>
#include <SFML\Graphics.hpp>
#include "SoundPlayer.h"
#include <fstream>

class Unit;
class Game;
class Item
{
public:

	enum RenderingModes
	{
		Above,
		Depth,
		Behind
	};

	// TODO Item needs clone functionality, prototype pattern, if we want to be able to inherit from item
	explicit Item(const std::string &itemName, const std::string &inventoryTextureName, const std::string &textureName, int id);
	virtual ~Item() {};

	// Throws in the item into orbit, marking itself for destruction once the moon has been passed
	void flyOff();
	void updateFlyoff(float deltaTime);

	// Updated when the item is held, when picked up from the inventory
	virtual void heldUpdate(float detlaTime, Game &game) {};
	// If the item needs custom interaction functionality
	virtual void update(float deltaTime, Game &game) {};

	virtual bool onCollisionWithUnit(Unit &unit, Game &game) { return false; }; // Called when a unit collides with the item, returning TRUE will destroy THIS item

	virtual bool onInteract(Item &otherItem, Game &game) { return false;  }; // Called when this item iteracts with another item, returning TRUE will destroy THIS item
	virtual bool onInteractedWith(Item &otherItem, Game &game) { return false; }; // Called when another item interactors with THIS item, returning TRUE will destroy THIS item
	virtual bool onSyncedWith(Item &otherItem) { return false; } // Called when another item with the same syncID is triggered, returning TRUE will destroy this item
	virtual bool onInteractUnit(Unit &unit) { return false; } // Called when this item interacts with a unit, returning TRUE will destroy this item

	virtual void onUse(Game &game) {};
	virtual void onExamine() {};
	virtual void onPickUp() {};

	// Base functionality for loading/saving ID, syncID and usability status to a file
	// These should be called with Item::serialize() etc if you don't want to save the base Item
	// settings yourself
	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	// Prototype pattern so we can clone item hierarchies
	virtual Item* clone() = 0;

	// Extra draw function that is only called when item is held in inventory
	virtual void heldDraw() {};
	virtual void inventoryDraw(bool drawInWorldSpace = false); // Draws the inventory sprite instead
	virtual void draw();

	virtual sf::Vector2f getSpeed() const;
	virtual void onPositionChanged() {};

	// Marks an item for destruction
	void markForDestruction();

	void setPosition(const sf::Vector2f &pos);
	sf::Vector2f getPosition() const;

	sf::Sprite& getSprite();
	sf::Sprite& getInventorySprite();
	
	std::string getName() const;
	int getID() const;
	void setSyncID(int id);
	int getSyncID() const;

	bool isMarkedForDestruction() const;
	bool isLootable() const;
	bool isUsable() const;
	/*virtual*/ bool isCollidable() const;
	bool isSolid() const;
	std::string getUseString() const;
	std::string getPickupString() const;
	std::string getExamineString() const;
	/*virtual*/ sf::FloatRect getCollisionBounds() const; // Will be zero if collidable is false
	sf::FloatRect getInteractBounds() const;
	sf::Vector2f getInteractDistance() const;

protected:

	// Interaction variables that should be set by the deriving class's constructor
	bool m_lootable; // Saved to file
	bool m_usable; // Saved to file
	bool m_collidable; // Saved to file
	bool m_solid;
	RenderingModes m_renderingMode; // Saved to file
	sf::Vector2f m_interactDistance; // Allowed X and Y distance from player to interact with this item
	sf::FloatRect m_interactBounds;
	sf::FloatRect m_collisionBounds;
	std::string m_useString;
	std::string m_pickupString;
	std::string m_examineString;

private:

	bool m_flyingOff;
	float m_flyingOffLifeTime;

	bool m_markedForDestruction;

	sf::Vector2f m_speed;
	std::string m_itemName;
	sf::Sprite m_sprite;
	sf::Sprite m_inventorySprite;
	int m_id; // Unique identifier for the item
	int m_syncID; // ID to connect the item to other items
	std::string m_description;
};

// Default item class, since the Item class is now abstract
class DefaultItem : public Item
{
public:

	explicit DefaultItem(const std::string &textureName, int id);

	virtual Item* clone();
};

#endif