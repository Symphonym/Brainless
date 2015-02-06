#ifndef INCLUDED_EDITOR_ZOMBIE_MODE
#define INCLUDED_EDITOR_ZOMBIE_MODE

#include <SFML\Graphics.hpp>
#include "EditorZombie.h"
#include "Level.h"
#include <vector>

class EditorZombieMode
{
public:
	EditorZombieMode();

	// Returns true if something changed (so editor knows if it needs to resave)
	bool events(const sf::Event &event, const sf::RenderWindow &editorWindow, Level &level);
	bool update(float deltaTime, const sf::RenderWindow &editorWindow);
	void draw();

private:
	// Display current task
	sf::Text m_infoText; 

	// Used as a preview to how it would look like if the Zombie was placed
	EditorZombie m_highlightSprite;
	bool zombie_created = false;
	EditorZombie m_createdZombie;
	

	const int Zombie_types = 3;
	std::vector<EditorZombie> m_zombieMasks;
};

#endif