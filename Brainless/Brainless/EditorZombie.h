#ifndef INCLUDED_EDITOR_ZOMBIE
#define INCLUDED_EDITOR_ZOMBIE

#include <SFML\Graphics.hpp>
//#include <string>

struct EditorZombie
{
	sf::Sprite sprite;
	//std::string textureName;
	int type = 0, distance = 0, facing = 1;
};

#endif