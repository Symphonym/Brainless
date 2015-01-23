#ifndef INCLUDED_EDITOR_H
#define INCLUDED_EDITOR_H

#include <SFML\Graphics.hpp>

class Editor
{
public:

	Editor();

	// Starts the engine
	void run();

private:

	sf::View m_camera;

	// Update loop
	void loop();

	sf::RenderWindow m_editor;
};

#endif