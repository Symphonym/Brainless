#ifndef INCLUDED_LADDER_ITEM_H
#define INCLUDED_LADDER_ITEM_H

#include "Item.h"

class LadderItem : public Item
{
public:

	LadderItem(const std::string &ladderTexture, int id, bool isFront = true);

	virtual void onUse(Game &game);
	virtual void update(float deltaTime, Game &game);

	virtual void serialize(std::ofstream &writer) const;
	virtual void deserialize(std::ifstream &reader);

	virtual bool onToggle(int direction);
	virtual std::string getToggleString() const;

	int getLadderLenght();
	std::string getLadderTextureString();
	void setLadderTexture(int ladderLenght,const std::string &ladderTexture);

	virtual Item* clone();

private:

	// Whether or not the ladder is in use, it's not usable if it is
	bool m_inUse;
	bool m_reachedStartPos;

	float m_lerpValue;
	//Texture information
	int m_ladderLenght;
	std::string m_ladderTexture;
	
	// Start position when end is the original start pos
	sf::Vector2f m_startStartPos;

	sf::Vector2f m_startPos;
	sf::Vector2f m_endPos;

};

#endif