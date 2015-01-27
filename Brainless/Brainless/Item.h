#ifndef INCLUDED_ITEM_H
#define INCLUDED_ITEM_H

#include <string>
#include <SFML\Graphics.hpp>

class Item
{
public:

	// TODO Item needs clone functionality, prototype pattern, if we want to be able to inherit from item
	explicit Item(const std::string &textureName, int id, int syncID);

	// If the item needs custom interaction functionality
	virtual void update() {};
	virtual void onInteract(Item &otherItem) {}; // Called when this item iteracts with another item
	virtual void onInteractedWith(Item &otherItem) {}; // Called when another item interactors with THIS item

	// Prototype pattern so we can clone item hierarchies
	virtual Item* clone() = 0;

	void draw();

	void setPosition(const sf::Vector2f &pos);
	sf::Vector2f getPosition() const;

	sf::Sprite& getSprite();
	
	int getID() const;
	void setSyncID(int id);
	int getSyncID() const;

private:

	sf::Sprite m_sprite;
	int m_id; // Unique identifier for the item
	int m_syncID; // ID to connect the item to other items
};

// Default item class, since the Item class is now abstract
class DefaultItem : public Item
{
public:

	explicit DefaultItem(const std::string &textureName, int id, int syncID = -1);

	virtual Item* clone();
};

#endif