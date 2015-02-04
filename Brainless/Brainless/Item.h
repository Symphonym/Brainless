#ifndef INCLUDED_ITEM_H
#define INCLUDED_ITEM_H

#include <string>
#include <SFML\Graphics.hpp>


// Data for combining items
struct CombineData
{
	CombineData(int targetIDParam, int productItemIDParam);

	int targetID; // The ID of the item this can be combined with
	int productItemID; // ID of the item that is crafted
};

class Item
{
public:

	// TODO Item needs clone functionality, prototype pattern, if we want to be able to inherit from item
	explicit Item(const std::string &textureName, int id, CombineData combineData = CombineData(-1, -1));

	// If the item needs custom interaction functionality
	virtual void update(float deltaTime) {};

	virtual bool onInteract(Item &otherItem) { return false;  }; // Called when this item iteracts with another item, returning TRUE will destroy THIS item
	virtual bool onInteractedWith(Item &otherItem) { return false;  }; // Called when another item interactors with THIS item, returning TRUE will destroy THIS item
	virtual bool onSyncedWith(Item &otherItem) { return false; } // Called when another item with the same syncID is triggered, returning TRUE will destroy this item

	virtual void onUse(const sf::RenderWindow &window) {};
	virtual void onExamine() {};
	virtual void onPickUp() {};

	// Prototype pattern so we can clone item hierarchies
	virtual Item* clone() = 0;

	virtual void draw();

	void setPosition(const sf::Vector2f &pos);
	sf::Vector2f getPosition() const;

	sf::Sprite& getSprite();
	
	int getID() const;
	void setSyncID(int id);
	int getSyncID() const;
	const CombineData& getCombineData() const;

	bool isLootable() const;
	bool isUsable() const;
	bool isCollidable() const;
	std::string getUseString() const;
	std::string getPickupString() const;
	std::string getExamineString() const;
	sf::FloatRect getCollisionBounds() const; // Will be zero if collidable is false

protected:

	// Interaction variables that should be set by the deriving class's constructor
	bool m_lootable;
	bool m_usable;
	bool m_collidable;
	sf::Vector2f m_collisionOffset;
	sf::Vector2f m_collisionSize;
	std::string m_useString;
	std::string m_pickupString;
	std::string m_examineString;

private:

	sf::Sprite m_sprite;
	int m_id; // Unique identifier for the item
	int m_syncID; // ID to connect the item to other items
	std::string m_description;
	CombineData m_combineData; // Data for combining (crafting) into other items
};

// Default item class, since the Item class is now abstract
class DefaultItem : public Item
{
public:

	explicit DefaultItem(const std::string &textureName, int id, CombineData combineData = CombineData(-1, -1));

	virtual Item* clone();
};

#endif