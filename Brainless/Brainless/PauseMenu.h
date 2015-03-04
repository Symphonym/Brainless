#ifndef INCLUDED_PAUSE_MENU_H
#define INCLUDED_PAUSE_MENU_H

#include "State.h"
#include "GUIElement.h"
#include <memory>
#include <array>

class PauseMenu : public State
{
public:

	PauseMenu(StateMachine &machine);

	virtual void update(float deltaTime);
	virtual void draw();

private:

	typedef std::unique_ptr<GUIElement> GuiPtr;

	std::array<GuiPtr, 6> m_buttons;

	// Complete dark texture covering screen, generated in code and thus not loaded
	sf::Texture m_backgroundTexture;
	sf::Sprite m_background;
};

#endif