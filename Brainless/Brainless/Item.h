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
	explicit Item(const std::string &textureName, int id, int syncID = -1, CombineData combineData = CombineData(-1, -1));

	// If the item needs custom interaction functionality
	virtual void update() {};
	virtual bool onInteract(Item &otherItem) { return false;  }; // Called when this item iteracts with another item, returning TRUE will destroy THIS item
	virtual bool onInteractedWith(Item &otherItem) { return false;  }; // Called when another item interactors with THIS item, returning TRUE will destroy THIS item
	virtual bool onSync(Item &otherItem) { return false; } // Called this item is used, returning TRUE will destroy this item
	virtual bool onSyncedWith(Item &otherItem) { return false; } // Called when another item with the same syncID is triggered, returning TRUE will destroy this item
	virtual std::string examine() const;
	virtual std::string onUse() const;
	virtual std::string onPickUp() const;

	// Prototype pattern so we can clone item hierarchies
	virtual Item* clone() = 0;

	void draw();

	void setPosition(const sf::Vector2f &pos);
	sf::Vector2f getPosition() const;

	sf::Sprite& getSprite();
	
	int getID() const;
	void setSyncID(int id);
	int getSyncID() const;
	const CombineData& getCombineData() const;

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

	explicit DefaultItem(const std::string &textureName, int id, int syncID = -1, CombineData combineData = CombineData(-1, -1));

	virtual Item* clone();
};

#endif