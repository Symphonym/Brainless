#ifndef INCLUDED_MAIN_MENU_H
#define INCLUDED_MAIN_MENU_H

#include <array>
#include <memory>
#include "State.h"
#include "GUIElement.h"

class MainMenu : public State
{
public:

	MainMenu(StateMachine &machine);

	virtual void update(float deltaTime);
	virtual void draw();

private:

	typedef std::unique_ptr<GUIElement> GuiPtr;

	std::array<GuiPtr, 2> m_buttons;

	// Whether or not the confirm window for a new game is shown
	bool m_newGameConfirmWindow;
	std::array<GuiPtr, 2> m_newGameButtons;
	sf::Sprite m_confirmBackground;
};

#endif