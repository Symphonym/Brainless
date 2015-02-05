#ifndef INCLUDED_EDITOR_ZOMBIE
#define INCLUDED_EDITOR_ZOMBIE

#include <SFML\Graphics.hpp>
//#include <string>

struct EditorZombie
{
	enum ZombieTypes
	{
		Walk = 0,
		Idle = 1
		// Etc
	};
	sf::Sprite sprite;
	//std::string textureName;
	int type = Walk, walk_distance = 0, facing = 1;
};

#endif