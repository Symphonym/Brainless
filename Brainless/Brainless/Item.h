#ifndef INCLUDED_ITEM_H
#define INCLUDED_ITEM_H

#include <string>
#include <SFML\Graphics.hpp>

class Item
{
public:

	// TODO Item needs clone functionality, prototype pattern, if we want to be able to inherit from item
	explicit Item(const std::string &textureName, int id, int syncID = -1);

	// If the item needs custom interaction functionality
	virtual void update() {};

	void setPosition(const sf::Vector2f &pos);
	sf::Vector2f getPosition() const;

	sf::Sprite& getSprite();
	int getID() const;
	int getSyncID() const;

private:

	sf::Sprite m_sprite;
	int m_id; // Unique identifier for the item
	int m_syncID; // ID to connect the item to other items
};

#endif