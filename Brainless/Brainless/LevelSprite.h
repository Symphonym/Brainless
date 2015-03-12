#ifndef INCLUDED_LEVEL_SPRITE
#define INCLUDED_LEVEL_SPRITE

#include <SFML\Graphics.hpp>
#include <string>

enum class LevelSpriteLayers
{
	Background = 0,
	Foreground = 1,
	Depth = 2
};

struct LevelSprite
{
	sf::Sprite sprite;
	std::string textureName;
	LevelSpriteLayers layer;
};

#endif