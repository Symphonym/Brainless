#ifndef INCLUDED_LEVEL_SPRITE
#define INCLUDED_LEVEL_SPRITE

#include <SFML\Graphics.hpp>
#include <string>

struct LevelSprite
{
	sf::Sprite sprite;
	std::string textureName;
	bool drawToForeground;
};

#endif