#ifndef INCLUDED_EDITOR_ZOMBIE
#define INCLUDED_EDITOR_ZOMBIE

#include <SFML\Graphics.hpp>

struct EditorZombie
{
	sf::Sprite sprite;
	int type = 0, walk_distance = 0, facing = 1;
};

#endif